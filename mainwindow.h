#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "subwindow.h"
#include <QButtonGroup>
#include <QVector>

enum class myerror
{
  buttongroup_init_error,
};


class mainwindow : public QWidget
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = 0);
    ~mainwindow();


   bool subwindows_show();

private:

    //核心部分 插件的加载
    void loadPlugins();
    void add_subwindow(QObject *plugin);

    //副窗口的二维数组指针
    QVector<QVector<QWidget*>>  subwindows;


    //上方按钮组
    QButtonGroup * buttongroup_top;

    //左侧按钮组
    QButtonGroup * buttongroup_left;

    int top = 0;
    int left = 0;

    //按钮组初始化
    bool buttongroup_init();

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);


 signals:
    void buttonClicked(int);

};

#endif // MAINWINDOW_H
