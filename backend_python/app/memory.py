from app.database import get_connection

def save_message(session_id: str, role: str, content: str):
    connection = get_connection()
    cursor = connection.cursor()

    cursor.execute(
        """
        INSERT INTO chat_history (session_id, role, content)
        VALUES (?, ?, ?)
        """,
        (session_id, role, content)
    )

    connection.commit()
    connection.close()


def add_note(content: str):
    connection = get_connection()
    cursor = connection.cursor()

    cursor.execute(
        """
        INSERT INTO notes (content)
        VALUES (?)
        """,
        (content,)
    )

    connection.commit()
    connection.close()

def get_notes():
    connection = get_connection()
    cursor = connection.cursor()

    cursor.execute(
        """
        SELECT id, content, created_at
        FROM notes
        ORDER BY id DESC 
        """
    )

    rows = cursor.fetchall()
    connection.close()

    return rows

def clear_note():
    connection = get_connection()
    cursor = connection.cursor()

    cursor.execute("DELETE FROM notes")

    connection.commit()
    connection.close()


