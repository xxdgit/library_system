#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>

namespace Ui {
class DBConnect;
}

class DBConnect : public QWidget
{
    Q_OBJECT

public:
    explicit DBConnect(QWidget *parent = 0);
    ~DBConnect();
    bool connect_db();
    void exit_connect();
    QString do_login(QString str);
    const QSqlDatabase get_database();

private:
    Ui::DBConnect *ui;
    const QString host_name = "127.0.0.1";
    const int port = 3306;
    const QString db_user_name = "root";
    const QString db_user_passwd = "*******";
    const QString db_name = "library_manager";
    QSqlDatabase db;
};

#endif // DBCONNECT_H
