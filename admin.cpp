#include "admin.h"
#include "ui_admin.h"
#include "login.h"
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    ui->stacked_widget->setCurrentIndex(1);
    database = QSqlDatabase::cloneDatabase(dbconn.get_database(),"admin_library");
    dbconn.exit_connect();
    connect(ui->listWidget_1,SIGNAL(currentRowChanged(int)),ui->stacked_widget,SLOT(setCurrentIndex(int)));
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_pushButton_clicked()
{
    this->close();
}

void Admin::set_adminid(QString id,QString pwd)
{
    admin_id = id;
    admin_pwd = pwd;
    init_ui();
}

void Admin::init_ui()
{
    this->setWindowTitle("管理员" + admin_id);
    ui->admin_id_le->setText(admin_id);
    ui->admin_id_le->setEnabled(false);
    ui->admin_oldpwd_le->setText(admin_pwd);
    ui->admin_oldpwd_le->setEnabled(false);
}

//更改管理员密码的点击事件
void Admin::on_admin_ok_btn_clicked()
{
    QString new_pwd = ui->admin_newpwd_le->text();
    QString sure_pwd = ui->admin_surepwd_le->text().trimmed();
    if (new_pwd != NULL && sure_pwd != NULL && new_pwd == sure_pwd)
    {
        if (database.open()){
            QSqlQuery query(database);
            QString sql_update = "update AdminInfo set password = " + new_pwd + " where AdminId = " + admin_id;
            if(query.exec(sql_update)){
                QMessageBox::warning(this,"提示","密码已更改，请使用新密码登录",QMessageBox::Yes);
                ui->admin_oldpwd_le->setText(new_pwd);
            }
            database.close();
        }else
            QMessageBox::warning(this,"提示","无法连接数据库，更改失败",QMessageBox::Yes);
    }else
        QMessageBox::warning(this,"提示","请确认输入内容正确",QMessageBox::Yes);
}

//书籍借出的点击事件
void Admin::on_out_ok_btn_clicked()
{
    QString book_name = ui->out_bookname_le->text();
    QString stu_id = ui->out_stuid_le->text();
    int acount = ui->out_book_acount->text().toInt();
    int temp = acount;
    if(book_name != NULL && stu_id != NULL && acount != 0){
        if(database.open()){        //能打开数据库则先执行查询
            QSqlQuery query(database);
            QString sql_select = "select RemainCount,BookCode from BookInfo where Name=\"" + book_name +"\"";
            qDebug() << sql_select;
            query.exec(sql_select);
            if(query.first()){            //若返回的结果为true，也就是书存在
                qDebug() << query.value(0).toInt();
                qDebug() << acount;
                QString book_code = query.value(1).toString(); //取出书名对应的ID
                if (query.value(0).toInt() < acount){      //比较库存中该书的余量是否足够
                    QMessageBox::warning(this,"提示","该书库存不足",QMessageBox::Yes);
                }else{
                    acount = query.value(0).toInt() - acount;
                    qDebug() << acount;
                    QString str_acount = QString::number(acount,10);
                    QString sql_update = "update BookInfo set RemainCount=" + str_acount + " where Name =\"" + book_name +"\"";
                    if(query.exec(sql_update)){  //书籍出库，需更新Student表和BorrowBookInfo表

                        //更新Student表
                        sql_select = "select BorrowBookCount from Student where StuId=" + stu_id;
                        query.exec(sql_select);
                        if (query.first()){
                            acount = query.value(0).toInt() + temp;
                            str_acount = QString::number(acount,10);
                            sql_update = "update Student set BorrowBookCount=" + str_acount + " where StuId=" + stu_id;
                            if(!query.exec(sql_update)){
                                qDebug() << "更新Student表失败";
                            }
                        }

                        //更新BorrowBookInfo表
                        int borrow_datetime =  QDateTime::currentDateTime().toTime_t();
                        QString str_datetime = QDateTime::fromTime_t(borrow_datetime).toString("yyyy-MM-dd hh:mm:ss");
                        QString return_datetime = QDateTime::fromTime_t(borrow_datetime + 2592000).toString("yyyy-MM-dd hh:mm:ss");
                        query.prepare("insert into BorrowBookInfo(BookCode,StuId,BorrowDate,ReturnDate) values(?,?,?,?)");
                        query.addBindValue(book_code);
                        query.addBindValue(stu_id);
                        query.addBindValue(str_datetime);
                        query.addBindValue(return_datetime);
                        if(!query.exec()){
                            qDebug() << "更新BorrowBookInfo表失败";
                        }

                        QMessageBox::warning(this,"提示","该书已出库",QMessageBox::Yes);
                    }
                }
            } else {
                QMessageBox::warning(this,"提示","您所输入的书籍不存在",QMessageBox::Yes);
            }
            database.close();
        } else
            QMessageBox::warning(this,"错误","无法连接数据库，更改失败",QMessageBox::Yes);
    } else
        QMessageBox::warning(this,"提示","请确认是否输入内容",QMessageBox::Yes);
}

//还书的点击事件
void Admin::on_in_ok_btn_clicked()
{
    QString book_code = ui->in_bookid_le->text();
    QString stu_id = ui->in_stuid_le->text();
    int acount = ui->in_book_acount->text().toInt();
    if (book_code != NULL && stu_id != NULL){
        if(database.open()){
            QSqlQuery query(database);
            QString sql_select = "select RemainCount from BookInfo where BookCode=" + book_code;
             qDebug() << sql_select;
             query.exec(sql_select);
             if (query.first()) {
                 acount += query.value(0).toInt();
                 QString str_acount = QString::number(acount,10);
                 QString sql_update = "update BookInfo set RemainCount=" + str_acount + " where BookCode=" + book_code;
                 if (query.exec(sql_update))
                     QMessageBox::warning(this,"提示","该书已入库",QMessageBox::Yes);
             }
             database.close();
        } else
            QMessageBox::warning(this,"错误","无法连接数据库，更改失败",QMessageBox::Yes);
    } else {
        QMessageBox::warning(this,"提示","请确认是否输入内容",QMessageBox::Yes);
    }
}

//创建用户的点击事件
void Admin::on_create_btn_clicked()
{
    //从界面里获取信息
    QString stu_id = ui->create_stuid_le->text();
    QString stu_name = ui->create_stuname_le->text();
    QString dep_id = ui->create_depid_le->text();
    QString spe_id = ui->create_speid_le->text();
    QString sex = ui->create_sex_le ->text();
    QString passwd = ui->create_initpwd_le->text();

    if (stu_id != NULL && stu_name != NULL && dep_id != NULL && spe_id != NULL && sex != NULL && passwd != NULL && stu_id == passwd){
        if (database.open()){
            QSqlQuery query(database);
            QString sql_insert = "insert into Student values(?,?,?,?,?,?,?);";
            query.prepare(sql_insert);
            query.addBindValue(stu_id.toInt());
            query.addBindValue(stu_name);
            query.addBindValue(sex);
            query.addBindValue(dep_id.toInt());
            query.addBindValue(spe_id.toInt());
            query.addBindValue(0);
            query.addBindValue(passwd);
            if (query.exec())
                QMessageBox::warning(this,"提示","创建用户成功",QMessageBox::Yes);
            else
                QMessageBox::warning(this,"错误","创建用户失败，请再次尝试",QMessageBox::Yes);
            database.close();
        }else
            QMessageBox::warning(this,"错误","无法连接数据库",QMessageBox::Yes);
    }else {
        QMessageBox::warning(this,"创建失败","请将内容补充完整或检查初始密码与学号是否一致",QMessageBox::Yes);
    }
}

//修改用户密码的点击事件
void Admin::on_revise_btn_clicked()
{
    QString stu_id = ui->revise_stuid_le->text();
    QString new_pwd = ui->revise_newpwd_le->text();
    QString new_pwd2 = ui->revise_newpwd_le2->text();

    if (stu_id != NULL && new_pwd != NULL && new_pwd2 != NULL && new_pwd == new_pwd2){
        if (database.open()){
            QSqlQuery query(database);
            QString sql_update = "update Student set password=" + new_pwd + " where StuId=" + stu_id;
            if(query.exec(sql_update)){
                QMessageBox::warning(this,"提示","修改密码成功",QMessageBox::Yes);
            } else {
                QMessageBox::warning(this,"错误","修改密码失败，请再次尝试",QMessageBox::Yes);
            }
            database.close();
        } else {
            QMessageBox::warning(this,"错误","无法连接数据库",QMessageBox::Yes);
        }
    } else {
        QMessageBox::warning(this,"修改失败","请将内容补充完整或检查密码是否一致",QMessageBox::Yes);
    }
}

void Admin::on_delete_btn_clicked()
{
    QString stu_id = ui->delete_stuid_le->text();
    QString stu_name = ui->delete_stuname_le->text();

    if (stu_id != NULL && stu_name != NULL){
        if (database.open()){
            QSqlQuery query(database);
            QString sql_select = "select Name from Student where StuId=" + stu_id;
            query.exec(sql_select);
            if (query.first()) {
                if (stu_name != query.value(0)){
                    QMessageBox::warning(this,"错误","学号与姓名非同一个人",QMessageBox::Yes);
                } else {
                    QString sql_delete = "delete from Student where StuId=" + stu_id;
                    if (query.exec(sql_delete)){
                        QMessageBox::warning(this,"提示","已删除",QMessageBox::Yes);
                    }
                }
            } else {
                QMessageBox::warning(this,"错误","该学号不存在",QMessageBox::Yes);
            }
            database.close();
        } else {
            QMessageBox::warning(this,"错误","无法连接数据库",QMessageBox::Yes);
        }
    } else {
        QMessageBox::warning(this,"删除失败","请将内容补充完整",QMessageBox::Yes);
    }
}

void Admin::on_admin_back_btn_clicked()
{
}
