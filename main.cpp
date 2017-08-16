#include "login.h"
#include "dbconnect.h"
#include "admin.h"
#include "student.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login w;
    w.show();

    return a.exec();
}
