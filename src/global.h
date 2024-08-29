#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

#define g_configName    "config.ini"    //配置文件名
#define g_resultSaveDirName "TestResult"  //测试结果保存路径
#define g_timer1Interval 1000   //一号定时器的触发时间间隔,单位ms
#define g_timer2Interval 100    //二号指令定时器的触发时间间隔,单位ms
#define g_preInWaterSpeed 1000  //预进水阶段的电机转速
#define g_defaultTemperature 22  //默认温度22

//下位机类型枚举值,从左至右依次为:进水泵,回水泵,,温度计,电子秤,流量计
enum LMMAC_TYPE{PANPUMP,PERPUMP,OUTWATERMAC,TEMPRATURE,ESCALE,FLOWMETER};

//打印日志类型
enum MSG_TYPE{SUCCESS,MSGFALSE,SYSTEM,FLOWMAC,TEMPTURE,ES};

//当前测试阶段,从左到右依次为停止测试阶段,回水等待阶段,回水阶段,预进水阶段,进水阶段
enum TEST_STATE{STOP,WAIT,OUTWATER,PREINWATER,INWATERTOTAL,INWATERTIME};

#endif // GLOBAL_H
