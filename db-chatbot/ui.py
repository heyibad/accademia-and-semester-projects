from typing import Optional
import chainlit as cl
import main
from db import get_or_create_user, get_create_chat, create_message, get_messages_by_chat_id, engine 
from uuid import UUID
from sqlmodel import Session
import json


@cl.password_auth_callback
async def auth_callback(email: str, password: str):
    print(f"Authenticating user {email} with password {password}")
    
    user = await get_or_create_user(
        session=Session(engine),
        email=email,
        password=password
    )
    
    if user:
        print(f"User {user.email} authenticated successfully")
        return cl.User(
            identifier=email, 
            metadata={"role": "user", "provider": "credentials", "user_id": str(user.id)}
        )
    return None

@cl.on_chat_start
async def start_chat():
    # Get the user from the cl.user session (which is populated by auth_callback)
    user = cl.user_session.get("user")
    if not user:
        print("Error: User not found in session")
        await cl.Message(content="Authentication error. Please try logging in again.").send()
        return
        
    email = user.identifier
    user_id = UUID(user.metadata["user_id"])
    
    print(f"User {email} with ID {user_id} is starting a chat.")
    
    # Create or get the chat
    chat = await get_create_chat(
        session=Session(engine),
        user_id=user_id
    )
    print(f"Chat {chat.id} created successfully")
    
    # Now set the session variables
    cl.user_session.set("chat_id", chat.id)
    cl.user_session.set("email", email)
    
    # Get messages history and convert to expected format
    db_messages = await get_messages_by_chat_id(chat_id=chat.id)
    print(f"Retrieved {len(db_messages)} messages from the database.")
    
    print(f"Messages in the database: {db_messages}")
   
    history = []
    
    if db_messages:
        for msg in db_messages:
            try:
                # Parse the content which should be a JSON string
                content_dict = json.loads(msg.content)
                history.append(content_dict)
            except json.JSONDecodeError:
                # Handle legacy messages or improperly formatted ones
                print(f"Warning: Could not parse message content: {msg.content}")
                # Create a basic message if parsing fails
                history.append({"role": "user" if msg.sender == email else "assistant", "content": msg.content})
    
    # Store formatted history in session
    cl.user_session.set("history", history)
    print(f"Session history set with {len(history)} messages.")
    
    if len(history) > 0:
        for msg in history:
            if msg["role"] == "user":
                await cl.Message(
                    content=f"User: {msg['content']}"
                ).send()
            else:
                await cl.Message(
                    content=f"Assistant: {msg['content']}"
                ).send()
    else:
     await cl.Message(
        content="Welcome to the Study Chatbot! How can I Guide you?").send()
    
@cl.on_message
async def handle_message(message: cl.Message):
    # Get session data
    history = cl.user_session.get("history") or []
    chat_id = cl.user_session.get("chat_id")
    email = cl.user_session.get("email")
    
    if not chat_id or not email:
        print("Error: Missing chat_id or email in session")
        await cl.Message(content="Session error. Please refresh and try again.").send()
        return
    
    print(f"chat_id: {chat_id}")
    print(f"email: {email}")
    print(f"Received message: {message.content}")
    
    # Add user message to history
    user_message = {"role": "user", "content": message.content}
    history.append(user_message)
    
    # Save user message to database
    create_message(
        chat_id=chat_id,
        sender=email,
        content=user_message
    )
    
    # Process the message with the LLM
    try:
        print("----------=---------------")
        print(f"Processing message with history: {history}")
        print("----------=---------------")
        result = await main.main(query=history)
        
        # Add assistant response to history
        assistant_message = {"role": "assistant", "content": result}
        history.append(assistant_message)
        
        # Save assistant message to database
        create_message(
            chat_id=chat_id,
            sender="assistant",
            content=assistant_message
        )
        
        # Update session history
        cl.user_session.set("history", history)
        
        # Send response to the user
        await cl.Message(content=str(result)).send()
        
    except Exception as e:
        print(f"Error processing message: {str(e)}")
        await cl.Message(content="Sorry, I encountered an error processing your request.").send()