#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(ClientManager& manager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client_manager(manager)
{
    ui->setupUi(this);

    connect(&client_manager, &ClientManager::clientListUpdated, this, &MainWindow::updateClientList);

    connect(&client_manager, &ClientManager::chatMessageReceived, this, &MainWindow::appendChatToList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateClientList(const QStringList &clients)
{
    ui->listWidgetClient->clear();
    ui->listWidgetClient->addItems(clients);
}

void MainWindow::appendChatToList(const QString &message)
{
    ui->listWidgetChat->addItem(message);
}


void MainWindow::on_pushButtonSend_clicked()
{
    if(!ui->lineEditChat->text().isEmpty())
    {
        client_manager.sendMessageToServer("CHAT", ui->lineEditChat->text());
        ui->lineEditChat->clear();
    }
}


void MainWindow::on_lineEditChat_returnPressed()
{
    if(!ui->lineEditChat->text().isEmpty())
    {
        client_manager.sendMessageToServer("CHAT", ui->lineEditChat->text());
        ui->lineEditChat->clear();
    }
}
