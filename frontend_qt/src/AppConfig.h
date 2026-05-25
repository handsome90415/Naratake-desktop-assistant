#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

class AppConfig
{
public:
    static AppConfig& instance();

    bool load();

    QString pythonPath() const;
    QString backendWorkingDir() const;
    QString backendHost() const;
    int backendPort() const;

    QString characterImagePath() const;
    int petWidth() const;
    int petHeight() const;
    int marginRight() const;
    int marginBottom() const;

private:
    AppConfig() = default;

    QString m_pythonPath = "C:/Users/hands/anaconda3/envs/geo/python.exe";
    QString m_backendWorkingDir = "D:/Naratake/backend_python";
    QString m_backendHost = "127.0.0.1";
    int m_backendPort = 8000;

    QString m_characterImagePath = "D:/Naratake/frontend_qt/assets/character.png";
    int m_petWidth = 420;
    int m_petHeight = 300;
    int m_marginRight = 30;
    int m_marginBottom = 80;
};

#endif
