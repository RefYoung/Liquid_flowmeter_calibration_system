#include "lmhandler.h"

LMHandler::LMHandler(QObject *parent)
    : QStandardItemModel{parent}
{
    init();//调用类初始化方法
}

//初始化函数,对象构建前最先调用的方法,请勿在此方法调用前调用任何自定义方法
void LMHandler::init()
{
    initTable();//初始化表结构
}

//表格结构初始化函数,可以清除表格数据并重置表结构
void LMHandler::initTable()
{
    //表头:名称 串口  地址  角色  备注
    //角色有:进水泵,回水泵,电子秤,温度计,流量计
    this->clear();
    this->setColumnCount(5);
    this->setHeaderData(0,Qt::Horizontal,"名称");
    this->setHeaderData(1,Qt::Horizontal,"串口");
    this->setHeaderData(2,Qt::Horizontal,"地址");
    this->setHeaderData(3,Qt::Horizontal,"角色");
    this->setHeaderData(4,Qt::Horizontal,"备注");
}

//打开下位机编辑窗口
void LMHandler::appendLMWindow(QStringList serialPorts)
{
    appendLM a(serialPorts);
    connect(&a,SIGNAL(appendLMSig(QString,QString,QString,QString,QString)),this,SLOT(appendLMSlot(QString,QString,QString,QString,QString)));
    a.exec();
}

//当接收到appendLM类对象发送的新建下位机信号时调用此函数
void LMHandler::appendLMSlot(QString name, QString serialName,QString address,QString role, QString remark)
{
    this->insertRow(this->rowCount());

    int row=this->rowCount()-1;
    this->setData(this->index(row,0),name);
    this->setData(this->index(row,1),serialName);
    this->setData(this->index(row,2),address);
    this->setData(this->index(row,3),role);
    this->setData(this->index(row,4),remark);
}
