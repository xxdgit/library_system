#include "student.h"
#include "ui_student.h"

#include <QDebug>
#include <QMessageBox>

Student::Student(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Student)
{
    ui->setupUi(this);
    database = QSqlDatabase::cloneDatabase(dbconn.get_database(),"clone_library");
    dbconn.exit_connect();
    contents_tv_query("select * from BookInfo;");
}

Student::~Student()
{
    if (!database.isOpen())
        database.close();           //断开连接
    delete model_select;
    delete model_contents;
    delete model_records;
    delete ui;
}

void Student::set_student(QString stu_id)
{
    this->stu_id = stu_id;
    init_ui();
    records_tv_query();
}

void Student::init_ui()
{
    this->setWindowTitle(this->stu_id);
    if (database.open()){
        QSqlQuery query(database);
        QString sql_select = "select Name,sex,DepId,SpeId,password from Student where StuId=" + stu_id;
        query.exec(sql_select);
        if (query.first()){
            ui->stuid_le->setText(stu_id);
            ui->stuname_le->setText(query.value(0).toString());
            ui->stu_sex->setText(query.value(1).toString());
            ui->stu_dep_le->setText(query.value(2).toString());
            ui->stu_spe_le->setText(query.value(3).toString());
            ui->stu_passwd->setText(query.value(4).toString());
            set_lineedit_unable();
        }
    }

}

//显示全部书籍
void Student::contents_tv_query(QString str)
{
    if(database.open()){                    //打开数据库连接
        model_contents = new QSqlQueryModel(ui->select_contents_tv);
    //    QString str = "select * from BookInfo;";
        model_contents->setQuery(str,database);
       set_header(*model_contents);
        ui->select_contents_tv->setModel(model_contents);

        database.close();
    } else {
        QMessageBox::warning(this,"提示","无法连接数据库",QMessageBox::Yes);
    }
}

void Student::set_header(QSqlQueryModel& model)
{
    model.setHeaderData(0,Qt::Horizontal, tr("编号"));
    model.setHeaderData(1,Qt::Horizontal, tr("书名"));
    model.setHeaderData(2,Qt::Horizontal, tr("作者"));
    model.setHeaderData(3,Qt::Horizontal, tr("出版社"));
    model.setHeaderData(4,Qt::Horizontal, tr("出版时间"));
    model.setHeaderData(5,Qt::Horizontal, tr("总量"));
    model.setHeaderData(6,Qt::Horizontal, tr("余量"));
}

//显示借阅记录
void Student::records_tv_query()
{
    if (database.open())
    {
        model_records = new QSqlQueryModel(ui->select_record_tv);
        QString str = "select StuId,BookCode,BorrowDate,ReturnDate from BorrowBookInfo where StuId=" + this->stu_id;
        model_records->setQuery(str,database);
        model_records->setHeaderData(0,Qt::Horizontal,tr("学号"));
        model_records->setHeaderData(1,Qt::Horizontal,tr("图书编号"));
        model_records->setHeaderData(2,Qt::Horizontal,tr("借阅日期"));
        model_records->setHeaderData(3,Qt::Horizontal,tr("归还日期"));

        ui->select_record_tv->setModel(model_records);
        ui->select_record_tv->setColumnWidth(0,90);
        ui->select_record_tv->setColumnWidth(1,150);
        ui->select_record_tv->setColumnWidth(2,230);
        ui->select_record_tv->setColumnWidth(3,230);

        database.close();
    } else {
        QMessageBox::warning(this,"提示","无法连接数据库",QMessageBox::Yes);
    }
}

//修改按钮的点击事件
void Student::on_stu_revise_btn_clicked()
{
    ui->stuname_le->setEnabled(true);
    ui->stu_sex->setEnabled(true);
    ui->stu_dep_le->setEnabled(true);
    ui->stu_spe_le->setEnabled(true);
    ui->stu_passwd->setEnabled(true);
    ui->stuname_le->setFocus();
}

void Student::set_lineedit_unable()
{
    ui->stuid_le->setEnabled(false);
    ui->stuname_le->setEnabled(false);
    ui->stu_sex->setEnabled(false);
    ui->stu_dep_le->setEnabled(false);
    ui->stu_spe_le->setEnabled(false);
    ui->stu_passwd->setEnabled(false);
}

//刷新按钮的点击事件
void Student::on_refresh_btn_clicked()
{
    records_tv_query();
}

//确定按钮的点击事件
void Student::on_stu_update_btn_clicked()
{
    stu_name = ui->stuname_le->text();
    sex = ui->stu_sex->text();
    dep_id = ui->stu_dep_le->text();
    spe_id = ui->stu_spe_le->text();
    password = ui->stu_passwd->text();

    if (stu_name != NULL && sex != NULL && dep_id != NULL && spe_id != NULL && password != NULL){
        if (database.open()){
            QSqlQuery query(database);
            QString sql_update = "update Student set Name=?,sex=?,DepId=?,SpeId=?,password=? where StuId=" + stu_id;
            query.prepare(sql_update);
            query.addBindValue(stu_name);
            query.addBindValue(sex);
            query.addBindValue(dep_id.toInt());
            query.addBindValue(spe_id.toInt());
            query.addBindValue(password);
            if (query.exec()){
                QMessageBox::warning(this,"提示","您的信息已修改",QMessageBox::Yes);
                set_lineedit_unable();
            } else {
                QMessageBox::warning(this,"提示","修改失败，请再次尝试",QMessageBox::Yes);
            }
            database.close();
        } else {
            QMessageBox::warning(this,"提示","无法连接数据库",QMessageBox::Yes);
        }
    } else {
        QMessageBox::warning(this,"提示","不允许内容为空",QMessageBox::Yes);
    }
}
//取消按钮的点击事件
void Student::on_stu_cancel_btn_clicked()
{
    init_ui();
}

//查找按钮的点击事件
void Student::on_select_btn_clicked()
{
    QString str = ui->bookname_le->text();
    if (str != NULL){
        if (database.open()){
            QString sql_select = "select * from BookInfo where Name LIKE \"%" + str +"%\"" ;
            qDebug() << sql_select;
            model_select = new QSqlQueryModel(ui->select_contents_tv);
            model_select->setQuery(sql_select,database);
            set_header(*model_select);
            ui->select_contents_tv->setModel(model_select);

            database.close();
        }else {
            QMessageBox::warning(this,"提示","无法连接数据库",QMessageBox::Yes);
        }

    } else {
        QMessageBox::warning(this,"提示","请输入书籍的关键字",QMessageBox::Yes);
    }
}

//返回按钮的点击事件
void Student::on_stu_back_btn_clicked()
{
    contents_tv_query("select * from BookInfo;");
}
