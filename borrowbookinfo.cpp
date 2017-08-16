#include "borrowbookinfo.h"
#include "ui_borrowbookinfo.h"

BorrowBookInfo::BorrowBookInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BorrowBookInfo)
{
    ui->setupUi(this);
}

BorrowBookInfo::~BorrowBookInfo()
{
    delete ui;
}
