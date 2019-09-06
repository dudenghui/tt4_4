#-------------------------------------------------
#
# Project created by QtCreator 2019-04-04T09:33:46
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += serialport
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tt4_4
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        module/netcommunication.cpp \
        module/database.cpp \
        module/serialcommunicate.cpp \
    ui_file_c/pitch.cpp \
    ui_file_c/rolling.cpp \
    dial.cpp \
    message_push.cpp

HEADERS += \
        mainwindow.h \
    pitch.h \
    rolling.h \
    dial.h \
    message_push.h

FORMS += \
        mainwindow.ui \
    message_push.ui

RESOURCES += \
    image/image.qrc \
    database/database.qrc \
    qss.qrc \
    icon.qrc
