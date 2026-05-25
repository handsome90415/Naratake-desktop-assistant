from pydantic import BaseModel
from fastapi import APIRouter

from app.command_engine import process_message
from app.memory import save_message

router = APIRouter()

class ChatRequest(BaseModel):
    message: str
    session_id: str = "default"


class ChatResponse(BaseModel):
    reply: str

@router.get("/health")
def health_check():
    return {
        "status": "ok",
        "model": "local",
        "message": "Naratake backend is running."
    }


@router.post("/chat", response_model=ChatResponse)
def chat(request: ChatRequest):
    user_message = request.message
    session_id = request.session_id

    save_message(session_id, "user", user_message)

    reply = process_message(user_message)

    save_message(session_id, "assistant", reply)

    return ChatResponse(reply=reply)
