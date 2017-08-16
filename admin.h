#ifndef ADMIN_H
#define ADMIN_H

#include "dbconnect.h"
#include <QDialog>


namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = 0);
    ~Admin();

    void set_adminid(QString id,QString pwd);
    void init_ui();

public:
    DBConnect dbconn;
    QSqlDatabase database;

private slots:
    void on_pushButton_clicked();

    void on_admin_ok_btn_clicked();

    void on_out_ok_btn_clicked();

    void on_in_ok_btn_clicked();

    void on_create_btn_clicked();

    void on_revise_btn_clicked();

    void on_delete_btn_clicked();

    void on_admin_back_btn_clicked();

private:
    Ui::Admin *ui;
    QString admin_id;
    QString admin_pwd;
};

#endif // ADMIN_H
