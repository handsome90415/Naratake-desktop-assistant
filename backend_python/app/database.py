import sqlite3
from pathlib import Path

# 取的目前這個檔案往上兩層的資料夾路徑，並存入 BASE_DIR
BASE_DIR = Path(__file__).resolve().parent.parent
DATA_DIR = BASE_DIR / 'data'
DB_PATH = DATA_DIR / 'assistant.db'

def get_connection():
    DATA_DIR.mkdir(exist_ok=True)

    return sqlite3.connect(DB_PATH)

def init_db():
    connection = get_connection()
    cursor = connection.cursor()

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS chat_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            session_id TEXT NOT NULL,
            role TEXT NOT NULL,
            content TEXT NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    """)

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            content TEXT NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    """)

    connection.commit()
    connection.close()
