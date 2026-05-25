#include "PetWindow.h"
#include "ChatWindow.h"

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include "AppConfig.h"

PetWindow::PetWindow(QWidget *parent)
    : QWidget(parent),
      chatWindow(nullptr)
{
    setWindowTitle("Desktop AI Companion Pet");

    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool
    );

    setAttribute(Qt::WA_TranslucentBackground);

    const int petWidth = AppConfig::instance().petWidth();
    const int petHeight = AppConfig::instance().petHeight();
    const int marginRight = AppConfig::instance().marginRight();
    const int marginBottom = AppConfig::instance().marginBottom();

    setFixedSize(petWidth, petHeight);

    characterLabel = new QLabel(this);

//     QPixmap pixmap("D:/Naratake/frontend_qt/asserts/character.jpg");
    QPixmap pixmap(AppConfig::instance().characterImagePath());

    if (pixmap.isNull()) {
        characterLabel->setText("Pet");
        characterLabel->setStyleSheet(
            "color: white;"
            "background-color: rgba(30, 30, 30, 180);"
            "border-radius: 20px;"
            "padding: 20px;"
            "font-size: 24px;"
        );
    } else {
        characterLabel->setPixmap(
            pixmap.scaled(
                petWidth,
                petHeight,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
    }

    characterLabel->setAlignment(Qt::AlignCenter);
    characterLabel->setFixedSize(petWidth, petHeight);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(characterLabel);
    layout->setContentsMargins(0, 0, 0, 0);

    QScreen *screen = QGuiApplication::primaryScreen();

    if (screen) {
        QRect available = screen->availableGeometry();

        int x = available.right() - width() - marginRight;
        int y = available.bottom() - height() - marginBottom;

        move(x, y);
    }
}

void PetWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void PetWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void PetWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (chatWindow && chatWindow->isVisible()) {
            hideChatWindow();
        } else {
            showChatWindow();
        }

        event->accept();
    }
}

void PetWindow::showChatWindow()
{
    if (!chatWindow) {
        chatWindow = new ChatWindow();
        chatWindow->resize(500, 600);
    }

    chatWindow->show();
    chatWindow->raise();
    chatWindow->activateWindow();
}

void PetWindow::hideChatWindow()
{
    if (chatWindow) {
        chatWindow->hide();
    }
}

void PetWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *openChatAction = menu.addAction("Open Chat");
    QAction *hideChatAction = menu.addAction("Hide Chat");
    menu.addSeparator();
    QAction *exitAction = menu.addAction("Exit");

    QAction *selectedAction = menu.exec(event->globalPos());

    if (selectedAction == openChatAction) {
        showChatWindow();
    } else if (selectedAction == hideChatAction) {
        hideChatWindow();
    } else if (selectedAction == exitAction) {
        qApp->quit();
    }
}
