
TEMPLATE      = lib
CONFIG       += plugin                          #动态插件
QT           += widgets
INCLUDEPATH  += ../../system_plugin             #头文件所在地址
HEADERS       = tz_3_static.h
SOURCES       = tz_3_static.cpp
TARGET        = $$qtLibraryTarget(tz_3_static)  #制定目标库名称
DESTDIR       = ../../Debug/subwindow                 #动态插件所在位置


target.path = ./$$pwd                           #项目路径设置
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!
