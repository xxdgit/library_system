#ifndef STUDENT_H
#define STUDENT_H

#include "dbconnect.h"
#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class Student;
}

class Student : public QDialog
{
    Q_OBJECT

public:
    explicit Student(QWidget *parent = 0);
    ~Student();
    void contents_tv_query(QString str);
    void records_tv_query();
    void set_student(QString stu_id);
    void init_ui();
    void set_lineedit_unable();
    void set_header(QSqlQueryModel& model);

public:
    DBConnect dbconn;
    QSqlDatabase database;

private slots:
    void on_stu_revise_btn_clicked();

    void on_refresh_btn_clicked();

    void on_stu_update_btn_clicked();

    void on_stu_cancel_btn_clicked();

    void on_select_btn_clicked();

    void on_stu_back_btn_clicked();

private:
    Ui::Student *ui;
    QString stu_id;
    QString stu_name;
    QString sex;
    QString dep_id;
    QString spe_id;
    QString password;
    QSqlQueryModel* model_contents;
    QSqlQueryModel* model_records;
    QSqlQueryModel* model_select;
//     DBConnect dbconn;
};

#endif // STUDENT_H
