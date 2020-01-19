#ifndef TZ_3_STATIC_H
#define TZ_3_STATIC_H

#include <QObject>
#include <QWidget>
#include <subwindow.h>
#include <QPushButton>
#include <QtPlugin>

//public QObject  要放在最前面  不然出错 鬼知道为什么
class TZ_3_static : public QObject, public subwindowInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "system_plugin.mainwidow.subwindow" FILE "tz_3_static.json")
    Q_INTERFACES(subwindowInterface)


public:
    bool init() override;
};

#endif // TZ_3_STATIC_H
