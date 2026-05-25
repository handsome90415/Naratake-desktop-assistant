import uvicorn

from main import app
from app.database import init_db


def main():
    # 確保資料庫和資料表存在
    init_db()

    uvicorn.run(
        app,
        host="127.0.0.1",
        port=8000,
        reload=False,
        log_level="info"
    )


if __name__ == "__main__":
    main()