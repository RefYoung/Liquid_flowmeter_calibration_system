#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>
#include "appendserialinfo.h"

class SerialPortHandler : public QStandardItemModel
{
    Q_OBJECT

private:
    //私有方法
    void init();//类初始化函数

public:
    SerialPortHandler(QObject* parent = nullptr);

    //公共方法
    void initTable();//初始化串口数据表格
    void appendSerialInfo();//添加串口信息
    void updateConnectState();//更新连串口的连通性信息
    //尝试打开串口,打开成功返回串口地址,打开失败返回空指针
    QSerialPort* connectSerialPort(QString name,QString baute,QString databit,QString paritybit,QString stopbit,QString flowcontrol);

private slots:
    //当用户设置好串口参数并确定后调用此槽函数
    void onAppednSerialPortInfo(QString name,QString baute,QString databit,QString paritybit,QString stopbit,QString flowcontrol,QString remark);
};

#endif // SERIALPORTHANDLER_H
