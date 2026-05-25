#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;
class ApiClient;
class QCloseEvent;
class QTimer;

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSendClicked();
    void onReplyReceived(const QString &reply);
    void onErrorOccurred(const QString &errorMessage);
    void onBackendReady();
    void onBackendNotReady(const QString &errorMessage);
    void checkBackendHealth();

private:
    QTextEdit *chatDisplay;
    QLineEdit *inputLine;
    QPushButton *sendButton;
    ApiClient *apiClient;
    QTimer *healthTimer;
    bool backendIsReady;

    void appendMessage(const QString &sender, const QString &message);
};

#endif
