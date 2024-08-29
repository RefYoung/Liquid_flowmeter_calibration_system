#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QObject>
#include "serialporthandler.h"
#include "lmhandler.h"
#include "optionhandler.h"

class ConfigHandler : public QObject
{
    Q_OBJECT
public:
    ConfigHandler(QObject* parent = nullptr);

    //配置名称
    QString configName;//当前配置的名称

    //全局设置部分
    QString cheManualTemperature;//是否启用手动矫正温度
    QString spinManualTemperature;//手动矫正温度值
    QString spinOutWaterThres;//回水下限阈值
    QString spinInWaterThres;//进水上限阈值

    //串口配置部分
    SerialPortHandler *serialPortHandler;


    //下位即配置部分
    LMHandler *lmHandler;


    //测试配置部分
    OptionHandler *optionHandler;
};

#endif // CONFIGHANDLER_H
