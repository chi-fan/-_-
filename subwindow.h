#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QtPlugin>

class subwindowInterface
{

public:
    //定义虚析构
    virtual ~subwindowInterface() {}

    //定义初始化函数
    virtual bool init() = 0;

    //


};



//定义唯一标识符


#define subwindowInterface_iid "system_plugin.mainwidow.subwindow"

Q_DECLARE_INTERFACE(subwindowInterface, subwindowInterface_iid)




#endif // SUBWINDOW_H
