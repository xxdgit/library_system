#-------------------------------------------------
#
# Project created by QtCreator 2017-08-09T12:47:09
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = library_manager
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    dbconnect.cpp \
    student.cpp \
    admin.cpp

HEADERS  += login.h \
    dbconnect.h \
    student.h \
    admin.h

FORMS    += login.ui \
    dbconnect.ui \
    student.ui \
    admin.ui

DISTFILES +=

RESOURCES += \
    resource.qrc
