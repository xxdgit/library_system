#include "dbconnect.h"
#include "ui_dbconnect.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

DBConnect::DBConnect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DBConnect)
{
    ui->setupUi(this);
}

DBConnect::~DBConnect()
{
  //  delete db;
    delete ui;
}

bool DBConnect::connect_db()
{

    db = QSqlDatabase::addDatabase("QMYSQL","library");
    db.setHostName(host_name);
    db.setPort(port);
    db.setUserName(db_user_name);
    db.setPassword(db_user_passwd);
    db.setDatabaseName(db_name);

    if (db.open())
    {
        qDebug() << "Connect to database succeed.";
        return true;
    } else {
        qDebug() << db.lastError();
        db.removeDatabase("library");
        return false;
    }
}

QString DBConnect::do_login(QString str)
{
    QString password = NULL;

    if (connect_db()){
        QSqlQuery query(db);
        query.exec(str);
        if(query.next()){
            password=query.value(0).toString();
            qDebug() << password;
            exit_connect();
            return password;
        } else {
            exit_connect();
            qDebug() << "NULL";
            return NULL;
        }
    } else {
         QMessageBox::warning(this, "错误", "无法连接数据库",QMessageBox::Yes);
         return false;
    }
}

const QSqlDatabase DBConnect::get_database()
{
    if(connect_db())
        return db;
}

void DBConnect::exit_connect()
{
    if (db.isOpen())
    {
        db.close();
        qDebug() << "已关闭数据库";
    }
}
