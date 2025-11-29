#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "client_manager.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(ClientManager& manager, QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::Dialog *ui;
    ClientManager& client_manager;
};

#endif // DIALOG_H
