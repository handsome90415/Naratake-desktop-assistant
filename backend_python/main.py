from fastapi import FastAPI
from app.api import router
from app.database import init_db

app = FastAPI(
    title = "Naratake Desktop AI assistance",
    description = "An AI assistance can help and company with you",
)

app.include_router(router)

@app.on_event("startup")
def startup_event():
    init_db()
