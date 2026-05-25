#include "ChatWindow.h"
#include "ApiClient.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCursor>

#include <QTimer>
#include <QCloseEvent>
#include <QDebug>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent),
      backendIsReady(false)
{
    setWindowTitle("Naratake Desktop Assistant");

    setStyleSheet(
       "QWidget {"
       "   background-color: #121212;"
       "}"
    );

    apiClient = new ApiClient(this);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet(
        "QTextEdit {"
        "   background-color: #1e1e1e;"
        "   color: #f0f0f0;"
        "   border: 1px solid #444;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "}"
    );



    inputLine = new QLineEdit(this);
    inputLine->setStyleSheet(
        "QLineEdit {"
        "   background-color: #2b2b2b;"
        "   color: white;"
        "   border: 1px solid #555;"
        "   border-radius: 6px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "}"
    );

    inputLine->setPlaceholderText("Waiting for local backend...");
    inputLine->setEnabled(false);

    sendButton = new QPushButton("Send", this);
    sendButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #3a7afe;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 8px 14px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #555;"
        "   color: #aaa;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5a8dff;"
        "}"
    );

    sendButton->setEnabled(false);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(inputLine);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chatDisplay);
    mainLayout->addLayout(inputLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(inputLine, &QLineEdit::returnPressed, this, &ChatWindow::onSendClicked);

    connect(apiClient, &ApiClient::replyReceived, this, &ChatWindow::onReplyReceived);
    connect(apiClient, &ApiClient::errorOccurred, this, &ChatWindow::onErrorOccurred);

    connect(apiClient, &ApiClient::backendReady, this, &ChatWindow::onBackendReady);
    connect(apiClient, &ApiClient::backendNotReady, this, &ChatWindow::onBackendNotReady);

    healthTimer = new QTimer(this);
    connect(healthTimer, &QTimer::timeout, this, &ChatWindow::checkBackendHealth);
    healthTimer->start(1000);

    appendMessage("System", "Starting local backend. Please wait...");
    checkBackendHealth();
}

void ChatWindow::onSendClicked()
{
    if (!backendIsReady) {
        appendMessage("System", "Backend is not ready yet. Please wait.");
        return;
    }

    QString message = inputLine->text().trimmed();

    if (message.isEmpty()) {
        return;
    }

    appendMessage("You", message);
    inputLine->clear();

    sendButton->setEnabled(false);
    inputLine->setEnabled(false);

    apiClient->sendMessage(message);
}

void ChatWindow::onReplyReceived(const QString &reply)
{
    appendMessage("Assistant", reply);

    inputLine->setEnabled(true);
    sendButton->setEnabled(true);
    inputLine->setFocus();
}

void ChatWindow::onErrorOccurred(const QString &errorMessage)
{
    appendMessage("Error", errorMessage);

    inputLine->setEnabled(true);
    sendButton->setEnabled(true);
    inputLine->setFocus();
}

void ChatWindow::appendMessage(const QString &sender, const QString &message)
{
    QString safeSender = sender.toHtmlEscaped();
    QString safeMessage = message.toHtmlEscaped();
    safeMessage.replace("\n", "<br>");

    QString color;

    if (sender == "You") {
        color = "#8ab4f8";
    } else if (sender == "Assistant") {
        color = "#b6f2c2";
    } else if (sender == "Error") {
        color = "#ff8a80";
    } else {
        color = "#cccccc";
    }

    QString html =
        "<div style='margin-bottom: 10px;'>"
        "<div style='font-weight: bold; color:" + color + ";'>" + safeSender + ":</div>"
        "<div style='color: #f0f0f0; line-height: 1.4;'>" + safeMessage + "</div>"
        "</div>";

    chatDisplay->append(html);

    QTextCursor cursor = chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    chatDisplay->setTextCursor(cursor);
}

void ChatWindow::checkBackendHealth()
{
    apiClient->checkHealth();
}

void ChatWindow::onBackendReady()
{
    if (!backendIsReady) {
        backendIsReady = true;

        inputLine->setEnabled(true);
        sendButton->setEnabled(true);
        inputLine->setPlaceholderText("Type a command, e.g. help, time, show notes...");

        appendMessage("System", "Local backend is ready. Try: help");
    }

    healthTimer->stop();
}

void ChatWindow::onBackendNotReady(const QString &errorMessage)
{
    if (!backendIsReady) {
        inputLine->setEnabled(false);
        sendButton->setEnabled(false);
        inputLine->setPlaceholderText("Waiting for local backend...");

        // 不要每秒洗版，只在 Application Output 看 debug 就好
        qDebug() << "Backend not ready:" << errorMessage;
    }
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
