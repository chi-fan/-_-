#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    try
//    {
        mainwindow w;
        w.show();
//    }
//    catch(int x)
//    {
//        qDebug()<<x;
//    }
//    catch(...)
//    {
//       QMessageBox::information(NULL, "错误", "未知错误", QMessageBox::Ok);
//    }



    return a.exec();
}
