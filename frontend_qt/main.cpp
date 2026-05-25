#include <QApplication>

#include "src/PetWindow.h"
#include "src/BackendManager.h"
#include "src/AppConfig.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setQuitOnLastWindowClosed(false);

    AppConfig::instance().load();

    BackendManager backendManager;
    backendManager.startBackend();

    PetWindow petWindow;
    petWindow.show();

    int result = app.exec();

    backendManager.stopBackend();

    return result;
}
