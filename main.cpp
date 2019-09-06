#include "mainwindow.h"
#include <QApplication>
#include "message_push.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    message_push qq;
//    qq.show();

    return a.exec();
}
