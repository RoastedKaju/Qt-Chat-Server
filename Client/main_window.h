#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "client_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ClientManager& manager, QWidget *parent = nullptr);
    ~MainWindow();

private:
    void updateClientList(const QStringList& clients);
    void appendChatToList(const QString& message);

private slots:

    void on_pushButtonSend_clicked();

    void on_lineEditChat_returnPressed();

private:
    Ui::MainWindow *ui;
    ClientManager& client_manager;
};
#endif // MAIN_WINDOW_H
