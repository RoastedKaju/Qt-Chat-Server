#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QMessageBox>

Dialog::Dialog(ClientManager& manager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , client_manager(manager)
{
    ui->setupUi(this);

    // Default values for fields
    ui->lineEditName->setText("User");
    ui->lineEditAddress->setText("127.0.0.1");
    ui->lineEditPort->setText("5500");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    // Check the fields
    const bool name_check = !ui->lineEditName->text().trimmed().isEmpty();
    const bool address_check = !ui->lineEditAddress->text().isEmpty();
    const bool port_check = !ui->lineEditPort->text().trimmed().isEmpty();

    if(!name_check || !address_check || !port_check)
    {
        qDebug() << "Some Field is invalid.";
        QMessageBox::warning(this, "Invalid Input", "Please Enter Correct Information");
        return;
    }

    // Check port
    bool ok;
    int port = ui->lineEditPort->text().trimmed().toInt(&ok);
    if(!ok || port < 1 || port > 65535)
    {
        QMessageBox::warning(this, "Invalid Port", "Please enter a valid port number (1-65535)");
        return;
    }

    // Try connect to server
    const QString address_value = ui->lineEditAddress->text();
    client_manager.connectToServer(address_value, port);

    // 2-Second time out
    if(!client_manager.getSocket()->waitForConnected(2000))
    {
        QMessageBox::critical(this, "Connection Failed", "Unable to connect to server.\nError: " + client_manager.getSocket()->errorString());
        return;   // stay in dialog
    }

    // Send the name to server
    client_manager.sendMessageToServer("SET_NAME", ui->lineEditName->text());

    accept();
}

void Dialog::on_buttonBox_rejected()
{
    reject();
}

