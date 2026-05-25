from datetime import datetime
from app.memory import add_note, get_notes, clear_note

def process_message(message: str) -> str:
    # 消除前後空白
    text = message.strip()

    if not text:
        return "Please type something."

    lower = text.lower()

    if lower == "help":
        return (
            "Available commands:\n"
            "- help\n"
            "- time\n"
            "- add note <your note>\n"
            "- show notes\n"
            "- clear notes\n"
            "- about"
        )

    if lower == "time":
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        return f"Current local time: {now}"

    if lower.startswith("add note"):
        note = text[len("add note "): ].strip()

        if not note:
            return "Your note is empty."

        add_note(note)

        return f"Note saved: {note}"

    if lower == "show notes":
        notes = get_notes()

        if not notes:
            return "No notes found."

        result = "Your notes:\n"

        for note_id, content, created_at in notes:
            result += f"{note_id}. {content} ({created_at}\n)"

        return result.strip()

    if lower == "clear notes":
        clear_note()

        return "All notes have been cleared."

    if lower == "about":
        return (
            "Naratale Desktop AI Assistant v_0.1\n"
            "This is a fully local desktop assistant backend."
        )

    return (
        "I don't understand your command.\n"
        "Type 'help' to see available commands."
    )


