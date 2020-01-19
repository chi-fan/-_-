#include "mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QPen>
#include <QPluginLoader>
#include <QApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>


static QString btn_style_left_on{" QPushButton"
                                 "{"
                                 "color:black;"
                                 "background-color:rgb(108, 166, 205);"
                                 "border: 1px;"
                                 "}"};

static QString btn_style_left{ "  QPushButton"
                               "{"
                               "color:black;"
                               "background-color:rgb(207,207, 207);"
                               //                    "border-radius:5px;"
                               "border: 1px;"
                               "}"

                               "  QPushButton:hover"
                               "{"
                               "color:black;"
                               "background-color:rgb(105,105, 105);"

                               "}"

                               " QPushButton:pressed"
                               "{"
                               "color:black;"
                               "padding-left:3px;"
                               "padding-top:3px;"
                               "}"};

static QString btn_style_top_on{" QPushButton"
                                "{"
                                "color:black;"
                                "background-color:rgb(108, 166, 205);"
                                "border: 1px;"
                                "}" };

static QString btn_style_top{" QPushButton"
                             "{"
                             "color:black;"
                             "background-color:rgb(190, 190, 190);"
                             //                    "border-radius:5px;"
                             "border: 1px;"
                             "}"

                             "  QPushButton:hover"
                             "{"
                             "color:black;"
                             "background-color:rgb(105,105, 105);"

                             "}"

                             " QPushButton:pressed"
                             "{"
                             "color:black;"
                             "padding-left:3px;"
                             "padding-top:3px;"
                             "}"};


mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent)
{
    this->resize(1280, 800);
    this->setMinimumWidth(1000);
    this->setMinimumHeight(800);                               //最小屏幕大小
    this->setWindowTitle("hello world");

    //上方按钮组
    buttongroup_top = new QButtonGroup(this);


    //左侧按钮组
    buttongroup_left = new QButtonGroup(this);

    //按钮组初始化
    if(!buttongroup_init())
    {
        QMessageBox::information(NULL, "错误", "主界面按钮组初始化失败",  QMessageBox::Ok);
        return;
    }


    connect(buttongroup_top, static_cast<void (QButtonGroup:: *)(int)>(&QButtonGroup::buttonClicked),
            [=](int x){
        int i = 0;
        for(auto tem_btn : buttongroup_top->buttons())
        {
            x == i++? tem_btn->setStyleSheet(btn_style_top_on): tem_btn->setStyleSheet(btn_style_top);
        }
        top = x;
        subwindows_show();
    });

    connect(buttongroup_left, static_cast<void (QButtonGroup:: *)(int)>(&QButtonGroup::buttonClicked),
            [=](int x){
        int i = 0;
        for(auto tem_btn : buttongroup_left->buttons())
        {
            x == i++? tem_btn->setStyleSheet(btn_style_left_on): tem_btn->setStyleSheet(btn_style_left);
        }
        left = x;
        subwindows_show();
    });


    loadPlugins();
}

mainwindow::~mainwindow()
{

}

//子屏幕选择显示
bool mainwindow::subwindows_show()
{
    for(auto x : subwindows)
    {
        for(auto y : x)
        {
            if(y)
            {
                y->hide();
            }
        }
    }

    if(subwindows[top][left])
    {
        subwindows[top][left]->show();
    }
    else
    {
        return false;
    }

    return true;
}

//加载插件
void mainwindow::loadPlugins()
{
    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());

    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();

    pluginsDir.cd("subwindow");  //进入subwidows文件夹
    // qDebug()<<pluginsDir;

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        qDebug()<< fileName << loader.metaData();
        QObject *plugin = loader.instance();
        if (plugin)
        {
            add_subwindow(plugin);
        }
    }

    subwindows_show();

}

void mainwindow::add_subwindow(QObject *plugin)
{
    int count_top = 0;
    int count_left = 0;
    // subwindows.clear();
    QDesignerCustomWidgetInterface *subwindow = qobject_cast<QDesignerCustomWidgetInterface *>(plugin);
    if(subwindow)
    {
        count_top = subwindow->whatsThis().section(",", 0 , 0).toInt();
        if(count_top >= subwindows.count())
        {
            QMessageBox::information(NULL, "错误", "上方按钮不够", QMessageBox::Ok);
            return ;
        }
        count_left = subwindow->whatsThis().section(",", 1 , 1).toInt();
        if(count_left >= subwindows[0].count())
        {
            QMessageBox::information(NULL, "错误", "左侧按钮不够", QMessageBox::Ok);
            return ;
        }
        qDebug()<< count_top << count_left << subwindows.count();
        subwindows[count_top][count_left] = subwindow->createWidget(this);
        subwindows[count_top][count_left]->hide();

    }
    else
    {
             QMessageBox::information(NULL, "错误", "加载失败", QMessageBox::Ok);
    }
}

//按钮组初始化和初始化子屏幕数组
bool mainwindow::buttongroup_init()
{
    QFile loadFile(QStringLiteral("TZ_3.json"));

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open TZ_3 file.");
        return false;
    }

    //获取配置文件中按钮数据 并设置按钮
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonArray buttonArray = loadDoc.object()["button_top"].toArray();
    for (int buttonIndex = 0; buttonIndex < buttonArray.size(); ++buttonIndex)
    {
        QPushButton* button = new QPushButton(this);
        QJsonObject buttonObject = buttonArray[buttonIndex].toObject();
        button->setText(buttonObject["text"].toString());
        // qDebug()<<buttonObject["text"].toString();
        buttongroup_top->addButton(button, buttonIndex);
    }
    buttonArray = loadDoc.object()["button_left"].toArray();
    for (int buttonIndex = 0; buttonIndex < buttonArray.size(); ++buttonIndex)
    {
        QPushButton* button = new QPushButton(this);
        QJsonObject buttonObject = buttonArray[buttonIndex].toObject();
        button->setText(buttonObject["text"].toString());
        //  qDebug()<<buttonObject["text"].toString();
        buttongroup_left->addButton(button, buttonIndex);
    }

    //设置按钮风格
    QFont font("黑体", 15, 2, false);
    int count_top = 0;
    for(auto tem_btn : buttongroup_top->buttons())
    {
        tem_btn->resize(200, 52);
        tem_btn->move(100 + (count_top++) * 200, 2);
        tem_btn->setFont(font);
        count_top == 1? tem_btn->setStyleSheet(btn_style_top_on): tem_btn->setStyleSheet(btn_style_top);
    }


    QFont font1("黑体", 12, 2, false);
    int count_left = 0;
    for(auto tem_btn : buttongroup_left->buttons())
    {
        tem_btn->resize(98, 40);
        tem_btn->move(1, 55 + (count_left++) * 40);
        tem_btn->setFont(font1);
        count_left == 1? tem_btn->setStyleSheet(btn_style_left_on ): tem_btn->setStyleSheet(btn_style_left);
    }

    //初始化子屏幕数组
    subwindows.resize(count_top);
    for (int i = 0; i < subwindows.count(); i++)
        subwindows[i].resize(count_left);

    for(int i = 0; i < count_top; i++)
    {
        for(int j = 0; j < count_left; j++)
        {
            subwindows[i][j] = NULL;
        }
    }

    return true;
}



void mainwindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    for(auto x : subwindows)
    {
        for(auto y : x)
        {
            if(y)
            {
                y->move(101, 56);
                y->resize(this->width()-102, this->height()-57);
            }
        }
    }
}




//给窗口加边框  加图表
void mainwindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainterPath path;
    path.addRect(0,0,this->width(),this->height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);    //使用pen要加上
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap);   //加cpp的边框
    painter.setPen(pen);
    painter.drawPath(path);

    QPainterPath path1;
    path1.addRect(1, 1, this->width()-2, 55);                 //上方矩形设计
    path1.setFillRule(Qt::WindingFill);
    painter.fillPath(path1,QBrush(QRgb(0xBEBEBE)));

    QPainterPath path2;                                         //左下方矩形设计
    path2.addRect(1, 55, 98, this->height()-56);
    path2.setFillRule(Qt::WindingFill);
    painter.fillPath(path2, QBrush(QRgb(0xCFCFCF)));

    QPen pen2(Qt::black, 1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen2);
    painter.drawLine(0, 55, this->width(), 55);                          //上横线
    painter.drawLine(100, 55, 100, this->height()-1);                    //左竖线


}
