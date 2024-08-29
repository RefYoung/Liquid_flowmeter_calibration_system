#ifndef TESTHANDLER_H
#define TESTHANDLER_H

#include <QObject>
#include <QSerialPort>
#include "global.h"

class TestHandler : public QObject
{
    Q_OBJECT
public:
    explicit TestHandler(QObject *parent = nullptr);

    QString name;//下位机名称
    LMMAC_TYPE macType;//下位机角色
    QString address;//下位机地址
    QString serialPortName;//下位机串口名
    QList<QByteArray>* cmdList;//下位机指令列表
signals:

};

#endif // TESTHANDLER_H
