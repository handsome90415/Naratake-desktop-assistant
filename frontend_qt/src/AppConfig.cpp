#include "AppConfig.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

AppConfig& AppConfig::instance()
{
    static AppConfig config;
    return config;
}

bool AppConfig::load()
{
    QString appDir = QCoreApplication::applicationDirPath();

    QStringList possiblePaths = {
        appDir + "/config.json",
        "D:/Naratake/frontend_qt/config.json"
    };

    QFile file;
    QString usedPath;

    for (const QString &path : possiblePaths) {
        if (QFile::exists(path)) {
            file.setFileName(path);
            usedPath = path;
            break;
        }
    }

    if (usedPath.isEmpty()) {
        qDebug() << "config.json not found. Using default config.";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open config.json:" << usedPath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        qDebug() << "Invalid config.json format.";
        return false;
    }

    QJsonObject root = doc.object();

    QJsonObject backend = root.value("backend").toObject();
    m_pythonPath = backend.value("pythonPath").toString(m_pythonPath);
    m_backendWorkingDir = backend.value("workingDir").toString(m_backendWorkingDir);
    m_backendHost = backend.value("host").toString(m_backendHost);
    m_backendPort = backend.value("port").toInt(m_backendPort);

    QJsonObject pet = root.value("pet").toObject();
    m_characterImagePath = pet.value("characterImage").toString(m_characterImagePath);
    m_petWidth = pet.value("width").toInt(m_petWidth);
    m_petHeight = pet.value("height").toInt(m_petHeight);
    m_marginRight = pet.value("marginRight").toInt(m_marginRight);
    m_marginBottom = pet.value("marginBottom").toInt(m_marginBottom);

    qDebug() << "Loaded config from:" << usedPath;
    return true;
}

QString AppConfig::pythonPath() const
{
    return m_pythonPath;
}

QString AppConfig::backendWorkingDir() const
{
    return m_backendWorkingDir;
}

QString AppConfig::backendHost() const
{
    return m_backendHost;
}

int AppConfig::backendPort() const
{
    return m_backendPort;
}

QString AppConfig::characterImagePath() const
{
    return m_characterImagePath;
}

int AppConfig::petWidth() const
{
    return m_petWidth;
}

int AppConfig::petHeight() const
{
    return m_petHeight;
}

int AppConfig::marginRight() const
{
    return m_marginRight;
}

int AppConfig::marginBottom() const
{
    return m_marginBottom;
}
