#include "ApiClient.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

ApiClient::ApiClient(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void ApiClient::sendMessage(const QString &message)
{
    QUrl url("http://127.0.0.1:8000/chat");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["message"] = message;
    json["session_id"] = "qt_frontend";

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray responseData = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            reply->deleteLater();
            return;
        }

        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (!responseDoc.isObject()) {
            emit errorOccurred("Invalid JSON response from backend.");
            reply->deleteLater();
            return;
        }

        QJsonObject obj = responseDoc.object();
        QString assistantReply = obj.value("reply").toString();

        emit replyReceived(assistantReply);

        reply->deleteLater();
    });
}


void ApiClient::checkHealth()
{
    QUrl url("http://127.0.0.1:8000/health");

    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray responseData = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            emit backendNotReady(reply->errorString());
            reply->deleteLater();
            return;
        }

        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (!responseDoc.isObject()) {
            emit backendNotReady("Invalid health response.");
            reply->deleteLater();
            return;
        }

        QJsonObject obj = responseDoc.object();
        QString status = obj.value("status").toString();

        if (status == "ok") {
            emit backendReady();
        } else {
            emit backendNotReady("Backend status is not ok.");
        }

        reply->deleteLater();
    });
}
