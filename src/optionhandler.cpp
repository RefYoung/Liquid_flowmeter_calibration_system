#include "optionhandler.h"

OptionHandler::OptionHandler(QObject *parent)
    : QStandardItemModel{parent}
{
    init();//调用对象构造初始化函数,请勿在此方法前调用任何自定义方法
}

//初始化表结构,删除所有数据
void OptionHandler::initTable()
{
    //表头:名称 模式  初始转速    时长(s)   终止转速    总量(ml)  重复次数    预进水时间(s)    停水等待时间(s)   备注
    this->clear();
    this->setColumnCount(10);
    this->setHeaderData(0,Qt::Horizontal,"名称");
    this->setHeaderData(1,Qt::Horizontal,"模式");
    this->setHeaderData(2,Qt::Horizontal,"初始转速");
    this->setHeaderData(3,Qt::Horizontal,"时长(s)");
    this->setHeaderData(4,Qt::Horizontal,"终止转速");
    this->setHeaderData(5,Qt::Horizontal,"总量(ml)");
    this->setHeaderData(6,Qt::Horizontal,"重复次数");
    this->setHeaderData(7,Qt::Horizontal,"预进水时间(s)");
    this->setHeaderData(8,Qt::Horizontal,"停水等待时间(s)");
    this->setHeaderData(9,Qt::Horizontal,"备注");
}

//打开测试方案对话框,并可能添加新测试配置
void OptionHandler::openAppendOptionWindow()
{
    appendOption* a = new appendOption;
    connect(a,SIGNAL(SignalAppendOption(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),this,SLOT(onAppendOption(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));
    a->exec();
    delete a;
}

//类对象初始函数,请勿在此函数前调用任何自定义方法
void OptionHandler::init()
{
    initTable();//初始化方案表格
}

//当appendOption类对象触发新建测试方案信号时,调用此槽函数
void OptionHandler::onAppendOption(QString name, QString mode, QString startSpeed, QString duration, QString endSpeed, QString total, QString times, QString preInWaterTime, QString waitTime, QString remark)
{
    this->insertRow(this->rowCount());

    int row=this->rowCount() - 1;
    this->setData(this->index(row,0),name);
    this->setData(this->index(row,1),mode);
    this->setData(this->index(row,2),startSpeed);
    this->setData(this->index(row,3),duration);
    this->setData(this->index(row,4),endSpeed);
    this->setData(this->index(row,5),total);
    this->setData(this->index(row,6),times);
    this->setData(this->index(row,7),preInWaterTime);
    this->setData(this->index(row,8),waitTime);
    this->setData(this->index(row,9),remark);
}
