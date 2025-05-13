from typing import Optional, cast
import chainlit as cl
import json
from uuid import UUID
from sqlmodel import Session
from db import get_or_create_user, get_create_chat, create_message, get_messages_by_chat_id, engine
from agents import Agent, Runner, AsyncOpenAI, OpenAIChatCompletionsModel, RunConfig
import os
from dotenv import load_dotenv, find_dotenv

# Load environment variables (only once)
load_dotenv(find_dotenv())

# Retrieve necessary environment variables
gemini_api_key = os.getenv("GEMINI_API_KEY")
model_name = os.getenv("MODEL_NAME", "gemini-2.0-flash")  # Use a default if not specified
base_url = os.getenv("LLM_BASE_URL", "https://generativelanguage.googleapis.com/v1beta/openai/")

if not gemini_api_key:
    raise ValueError("GEMINI_API_KEY is not set. Please ensure it is defined in your .env file.")

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
    
    # Set up the LLM client and config
    external_client = AsyncOpenAI(
        api_key=gemini_api_key,
        base_url=base_url,
    )
    
    model = OpenAIChatCompletionsModel(
        model=model_name,
        openai_client=external_client
    )
    
    config = RunConfig(
        model=model,
        model_provider=external_client,
        tracing_disabled=True
    )
    
    # Create the agent
    agent = Agent(
        name="Study Assistant",
        instructions="""
        You are a helpful study assistant that helps users with their academic questions.
        Focus only on educational and study-related topics. Be informative, accurate, and supportive.
        Provide detailed explanations that help the user understand complex topics.
        Avoid giving assistance on non-academic subjects.
        Use examples and analogies to make concepts clearer.
        """,
        model=model
    )
    
    # Now set all session variables
    cl.user_session.set("chat_id", chat.id)
    cl.user_session.set("email", email)
    cl.user_session.set("agent", agent)
    cl.user_session.set("config", config)
    
    # Get messages history and convert to expected format
    db_messages = await get_messages_by_chat_id(chat_id=chat.id)
    print(f"Retrieved {len(db_messages)} messages from the database.")
    
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
                    content=msg['content']
                ).send()
            else:
                await cl.Message(
                    content=msg['content'],
                    author="Assistant"
                ).send()
    else:
        await cl.Message(content="Welcome to the Study Chatbot! How can I Guide you?").send()

@cl.on_message
async def handle_message(message: cl.Message):
    """Process incoming messages and generate responses."""
    # Get session data
    history = cl.user_session.get("history") or []
    chat_id = cl.user_session.get("chat_id")
    email = cl.user_session.get("email")
    agent = cast(Agent, cl.user_session.get("agent"))
    config = cast(RunConfig, cl.user_session.get("config"))
    
    if not chat_id or not email or not agent or not config:
        print("Error: Missing session data")
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
        content=json.dumps(user_message)  # Ensure we're saving JSON string
    )
    
    # Create a message for streaming responses
    msg = cl.Message(content="", author="Assistant")
    await msg.send()
    
    try:
        print("\n[CALLING_AGENT_WITH_CONTEXT]\n", history, "\n")
        
        # Run the agent with streaming enabled
        result = Runner.run_streamed(agent, history, run_config=config)
        
        full_response = ""
        # Stream the response token by token
        async for event in result.stream_events():
            if event.type == "raw_response_event" and hasattr(event.data, 'delta'):
                token = event.data.delta
                full_response += token
                await msg.stream_token(token)
        
        # Add assistant response to history
        assistant_message = {"role": "assistant", "content": full_response}
        history.append(assistant_message)
        
        # Save assistant message to database
        create_message(
            chat_id=chat_id,
            sender="assistant",
            content=json.dumps(assistant_message)  # Ensure we're saving JSON string
        )
        
        # Update session history
        cl.user_session.set("history", history)
        
        # Log the interaction
        print(f"User: {message.content}")
        print(f"Assistant: {msg.content}")
        
    except Exception as e:
        print(f"Error processing message: {str(e)}")
        await msg.update(content=f"Sorry, I encountered an error: {str(e)}")