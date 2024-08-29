#ifndef LMHANDLER_H
#define LMHANDLER_H

#include <QStandardItemModel>
#include "appendlm.h"

class LMHandler : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit LMHandler(QObject *parent = nullptr);

private:
    //私有方法
    void init();//类初始化函数

public:
    //公有方法
    void initTable();//表格结构初始化函数
    void appendLMWindow(QStringList serialPorts);//打开下位机编辑窗口

private slots:
    //私有槽函数
    void appendLMSlot(QString,QString,QString,QString,QString);//当接收到创建下位机信号时,使用此槽函数
};

#endif // LMHANDLER_H
