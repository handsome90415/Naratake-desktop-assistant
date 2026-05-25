#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include <QObject>
#include <QProcess>

class BackendManager : public QObject
{
    Q_OBJECT

public:
    explicit BackendManager(QObject *parent = nullptr);
    ~BackendManager();

    void startBackend();
    void stopBackend();

private:
    QProcess *backendProcess;
};

#endif
