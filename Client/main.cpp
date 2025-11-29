#include "main_window.h"
#include "dialog.h"
#include "client_manager.h"

#include <QApplication>
#include <QDialog>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Handles our connection with server
    ClientManager client_manager;

    Dialog login_dialog(client_manager);
    if(login_dialog.exec() == QDialog::Accepted)
    {
        MainWindow mainWindow(client_manager);
        mainWindow.show();
        return a.exec();
    }

    return 0;
}
