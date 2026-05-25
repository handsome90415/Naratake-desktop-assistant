#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ApiClient : public QObject
{
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);

    void sendMessage(const QString &message);
    void checkHealth();

signals:
    void replyReceived(const QString &reply);
    void errorOccurred(const QString &errorMessage);
    void backendReady();
    void backendNotReady(const QString &errorMessage);

private:
    QNetworkAccessManager *manager;
};

#endif
