import os
import asyncio
import chainlit as cl
from dotenv import load_dotenv, find_dotenv
from openai import AsyncOpenAI
from agents import (
    Agent,
    Runner,
    function_tool,
    set_default_openai_api,
    set_default_openai_client,
    set_tracing_disabled,
    OpenAIChatCompletionsModel
)
import traceback
from typing import Union, Dict, List, Any

# Load environment variables (only once)
load_dotenv(find_dotenv())

# Retrieve necessary environment variables
gemini_api_key = os.getenv("GEMINI_API_KEY")
model_name = os.getenv("MODEL_NAME")
base_url = os.getenv("LLM_BASE_URL")

if not gemini_api_key or not model_name or not base_url:
    raise Exception("Gemini API key or credentials not found in .env file")

# Create an instance of the external LLM client (Gemini)
client = AsyncOpenAI(
    api_key=gemini_api_key,
    base_url=base_url
)

# Set up the bot configuration
set_default_openai_client(client=client, use_for_tracing=False)
set_default_openai_api("chat_completions")
set_tracing_disabled(disabled=True)


# Define an async helper to run the agent with conversation history
async def agent_run(agent: Agent, history: List[Dict[str, str]]):
    try:
        # Get the current message from chainlit context for streaming
        msg = cl.Message.get_current()
        
        # Run the agent with streaming
        res = await Runner.run_streamed(agent, input=history)
        
        # Track full response
        full_response = ""
        
        # Stream the response token by token
        async for event in res.stream_events():
            if event.type == "raw_response_event" and hasattr(event.data, 'delta'):
                token = event.data.delta
                full_response += token
                
                # Stream token to UI
                if msg:
                    await msg.stream_token(token)
                    
        # Return the complete response
        return full_response
                
    except Exception as e:
        print(f"Error running agent: {str(e)}")
        traceback.print_exc()
        return None


# Create the talking agent instance with improved instructions
talking_agent = Agent(
    name="Study Assistant",
    instructions="""
    You are a helpful study assistant that helps users with their academic questions.
    Focus only on educational and study-related topics. Be informative, accurate, and supportive.
    Provide detailed explanations that help the user understand complex topics.
    Avoid giving assistance on non-academic subjects.
    Use examples and analogies to make concepts clearer.
    """,
    model=model_name
)


async def main(query: Union[str, List[Dict[str, str]]]) -> str:
    """
    Process a query using the talking agent.
    
    Args:
        query: Either a string message or a list of message dictionaries with role and content
        
    Returns:
        The agent's response text
    """
    try:
        # Handle different input types
        if isinstance(query, str):
            # Convert single string query to proper message format
            formatted_query = [{"role": "user", "content": query}]
        else:
            # Assume it's already a properly formatted list of message dicts
            formatted_query = query
        
        # Run the agent
        response = await agent_run(talking_agent, formatted_query)
        
        if response:
            return response
        else:
            return "I'm sorry, I encountered an error processing your request."
            
    except Exception as e:
        print(f"Error in main function: {str(e)}")
        traceback.print_exc()
        return "I apologize, but I'm having trouble processing your request right now."