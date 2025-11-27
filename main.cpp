#include <QCoreApplication>

#include "server_manager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create Server Manager
    ServerManager server_manager;
    server_manager.setupServer(5500);

    return a.exec();
}
