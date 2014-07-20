#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QStringList>
//#include "connectionmanager.h"
#include "ui_login.h"



namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    QString linePassword() const
    {
        return ui->lineEdit_Password->text();
    }

private slots:

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
