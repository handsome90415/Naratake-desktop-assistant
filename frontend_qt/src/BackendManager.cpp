#include "BackendManager.h"
#include "AppConfig.h"
#include <QCoreApplication>
#include <QFile>
#include <QDebug>

BackendManager::BackendManager(QObject *parent)
    : QObject(parent)
{
    backendProcess = new QProcess(this);
}

BackendManager::~BackendManager()
{
    stopBackend();
}

void BackendManager::startBackend()
{
    if (backendProcess->state() != QProcess::NotRunning) {
        qDebug() << "Backend is already running.";
        return;
    }

    QString appDir = QCoreApplication::applicationDirPath();
    QString packagedBackend = appDir + "/backend/backend.exe";

    QString program;
    QStringList arguments;
    QString workingDir;

    if (QFile::exists(packagedBackend)) {
        // Packaged mode
        program = packagedBackend;
        workingDir = appDir + "/backend";

        qDebug() << "Starting packaged backend:" << program;
    } else {
        // Development mode
        program = AppConfig::instance().pythonPath();
        workingDir = AppConfig::instance().backendWorkingDir();

//         arguments << "-m"
//                   << "uvicorn"
//                   << "main:app"
//                   << "--host"
//                   << AppConfig::instance().backendHost()
//                   << "--port"
//                   << QString::number(AppConfig::instance().backendPort());

        arguments << "run_backend.py";

        qDebug() << "Starting development backend with Python:" << program;
    }


    backendProcess->setWorkingDirectory(workingDir);

    backendProcess->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(backendProcess, &QProcess::readyReadStandardOutput, [this]() {
        qDebug() << "[Backend]" << backendProcess->readAllStandardOutput();
    });

    QObject::connect(backendProcess, &QProcess::errorOccurred, [](QProcess::ProcessError error) {
        qDebug() << "Backend process error:" << error;
    });

    QObject::connect(backendProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "Backend finished. Exit code:" << exitCode
                 << "Exit status:" << exitStatus;
    });

    backendProcess->start(program, arguments);

    if (!backendProcess->waitForStarted(3000)) {
        qDebug() << "Failed to start backend:" << backendProcess->errorString();
    } else {
        qDebug() << "Backend started.";
    }
}

void BackendManager::stopBackend()
{
    if (backendProcess->state() != QProcess::NotRunning) {
        qDebug() << "Stopping backend...";

        backendProcess->terminate();

        if (!backendProcess->waitForFinished(3000)) {
            qDebug() << "Backend did not stop gracefully. Killing it.";
            backendProcess->kill();
            backendProcess->waitForFinished();
        }

        qDebug() << "Backend stopped.";
    }
}
