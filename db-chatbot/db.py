from sqlmodel import SQLModel, create_engine, Session, select
from typing import List, Optional, Dict, Union, Any
from uuid import uuid4, UUID
from datetime import datetime
from sqlmodel import SQLModel, Field, Relationship
import os
import json


class User(SQLModel, table=True):
    __tablename__: str = "users"

    id: UUID = Field(default_factory=uuid4, primary_key=True)
    email: Optional[str] = Field(default=None, index=True, unique=True)
    username: Optional[str] = Field(default=None, index=True, unique=True)
    password: str = Field(nullable=False)  # This should store hashed passwords
    created_at: datetime = Field(default_factory=datetime.utcnow, nullable=False)
    updated_at: Optional[datetime] = None

    # Relationship to chats
    chats: List["Chat"] = Relationship(back_populates="user")


class Chat(SQLModel, table=True):
    __tablename__: str = "chats"

    id: UUID = Field(default_factory=uuid4, primary_key=True)
    user_id: UUID = Field(foreign_key="users.id", nullable=False)
    started_at: datetime = Field(default_factory=datetime.utcnow, nullable=False)
    last_msg_at: Optional[datetime] = Field(default_factory=datetime.utcnow)

    # Relationships
    user: Optional[User] = Relationship(back_populates="chats")
    messages: List["Message"] = Relationship(back_populates="chat")


class Message(SQLModel, table=True):
    __tablename__: str = "messages"

    id: UUID = Field(default_factory=uuid4, primary_key=True)
    chat_id: UUID = Field(foreign_key="chats.id", nullable=False)
    sender: str = Field(nullable=False)
    content: str = Field(nullable=False)  # JSON string
    message_metadata: Optional[str] = Field(default=None)
    created_at: datetime = Field(default_factory=datetime.utcnow, nullable=False)

    # Relationship back to chat
    chat: Optional[Chat] = Relationship(back_populates="messages")


DATABASE_URL = os.getenv("DB_URI")

if DATABASE_URL is None:
    raise ValueError("DB_URI environment variable is not set")

engine = create_engine(DATABASE_URL, echo=True)


def create_db():
    SQLModel.metadata.create_all(engine)


def create_message(chat_id: UUID, sender: str, content: Union[str, Dict[str, Any]]) -> Message:
    """
    Create a new message in the database.
    
    Args:
        chat_id: UUID of the chat
        sender: Email or identifier of the sender
        content: Message content as string or dictionary
        
    Returns:
        The created Message object
    """
    # Convert dict to JSON string if content is a dictionary
    if isinstance(content, dict):
        content = json.dumps(content)
        
    with Session(engine) as session:
        # Update last_msg_at time for the chat
        statement = select(Chat).where(Chat.id == chat_id)
        chat = session.exec(statement).first()
        if chat:
            chat.last_msg_at = datetime.utcnow()
            
        # Create the new message
        new_message = Message(
            chat_id=chat_id,
            sender=sender,
            content=content
        )
        session.add(new_message)
        session.commit()
        session.refresh(new_message)
        
    return new_message


async def get_messages_by_chat_id(chat_id: UUID) -> List[Message]:
    """
    Retrieve all messages for a given chat_id.
    
    Args:
        chat_id: UUID of the chat
        
    Returns:
        List of Message objects
    """
    with Session(engine) as session:
        statement = select(Message).where(Message.chat_id == chat_id).order_by(Message.created_at)
        messages = session.exec(statement).all()
    return messages


# Simple password comparison without encryption
def verify_password(stored_password: str, provided_password: str) -> bool:
    """Verify a stored password against one provided by user"""
    return stored_password == provided_password


async def get_or_create_user(session: Session, email: str, password: str) -> Optional[User]:
    """
    Look up a User by email; if found check password, if not create a new user.
    
    Args:
        session: Database session
        email: User's email
        password: User's password (stored as plaintext)
        
    Returns:
        User object if authentication successful, None otherwise
    """
    statement = select(User).where(User.email == email)
    existing = session.exec(statement).first()
    
    if existing:
        # Check password for existing user with simple comparison
        if verify_password(existing.password, password):
            return existing
        else:
            print(f"Invalid password for user {email}")
            return None
    
    # Create new user with plaintext password
    try:
        new_user = User(email=email, password=password)
        session.add(new_user)
        session.commit()
        session.refresh(new_user)
        return new_user
    except Exception as e:
        print(f"Error creating user: {str(e)}")
        session.rollback()
        return None


async def get_create_chat(session: Session, user_id: UUID) -> Chat:
    """
    Look up a Chat by user_id; if not found, create & return a new one.
    
    Args:
        session: Database session
        user_id: UUID of the user
        
    Returns:
        Chat object
    """
    statement = select(Chat).where(Chat.user_id == user_id).order_by(Chat.last_msg_at.desc())
    existing = session.exec(statement).first()
    
    if existing:
        return existing

    new_chat = Chat(user_id=user_id, last_msg_at=datetime.utcnow())
    session.add(new_chat)
    session.commit()
    session.refresh(new_chat)
    return new_chat


if __name__ == "__main__":
    # Create the database and tables
    create_db()