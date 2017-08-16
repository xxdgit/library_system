#include "login.h"
#include "ui_login.h"

#include "dbconnect.h"
#include "student.h"
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPair>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    init();
    connect(ui->login_btn,SIGNAL(clicked()),this,SLOT(login()));
}

Login::~Login()
{
    delete validator;
    delete ui;
}

void Login::login()
{
   verify_info();
}

//设置取消button
void Login::on_cancel_btn_clicked()
{
    this->close();
}

//设置登录用户名只能为数字（管理员编号或学号）
void Login::init()
{
    QRegExp re("^[0-9]{1,10}$");
    validator = new QRegExpValidator(re,0);
    ui->login_name_cb->lineEdit()->setValidator(validator);
}

//获取登录界面的用户信息
void Login::get_info()
{
    is_admin = ui->Admin_or_Stu_cb->currentText().trimmed();
    qDebug() << is_admin;
    login_name = ui->login_name_cb->currentText();
    qDebug() << login_name;
    password = ui->login_passwd_le->text();
    qDebug() <<password;
}

//判断是否满足登录条件
void Login::verify_info()
{
    get_info();
    if (login_name == NULL || password == NULL){
        QMessageBox::warning(this, "警告", "用户名或密码不能为空",QMessageBox::Yes);
    } else {
        if (is_admin == "管理人员"){
            QString str = "SELECT password from AdminInfo where AdminId=" + login_name + ";";
            if (type_select(str)) {
                this->close();
                Admin admin;
                admin.set_adminid(login_name, password);
                admin.show();
                admin.exec();
            } else {
                QMessageBox::warning(this, "错误", "请确认用户名或密码是否正确",QMessageBox::Yes);
            }
        } else if (is_admin == "学生用户") {
            QString str = "SELECT password from Student where StuId=" + login_name + ";";
            if (type_select(str)) {
                this->close();
                Student student;
                student.set_student(login_name);
                student.show();
                student.exec();
            } else {
                QMessageBox::warning(this, "错误", "请确认用户名或密码是否正确",QMessageBox::Yes);
            }
        }
    }
}

bool Login::type_select(QString str)
{
    qDebug() << str;
    QString pwd=dbconn.do_login(str);
    if(password == pwd)
        return true;
    else {
        qDebug() << "用户不存在或密码错误";
         return false;
    }
}


