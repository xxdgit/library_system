#ifndef LOGIN_H
#define LOGIN_H

#include "admin.h"
#include "dbconnect.h"
#include <QWidget>
#include <QString>
#include <QValidator>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void get_info();
    void init();
    void verify_info();
    bool type_select(QString str);

public:
    QValidator *validator;

signals:

private slots:
    void login();
    void on_cancel_btn_clicked();

private:
    Ui::Login *ui;

    QString login_name;
    QString password;
    QString is_admin;
    DBConnect dbconn;
};

#endif // LOGIN_H
