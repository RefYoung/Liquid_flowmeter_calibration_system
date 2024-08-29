#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>
#include "serialporthandler.h"
#include <QInputDialog>
#include "lmhandler.h"
#include "optionhandler.h"
#include <QSettings>
#include "global.h"
#include <QFile>
#include "confighandler.h"
#include <QCloseEvent>
#include <QTextBrowser>
#include <QDateTime>
#include <QDir>
#include "testhandler.h"
#include <QMap>
#include <QTimer>
#include <QtXlsx>
#include <QProgressBar>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAppendSerial_clicked();

    void on_btnDeleteSerial_clicked();

    void on_actConfig_triggered();

    void on_btnCheckConnectSerial_clicked();

    void on_btnDeleteAllSerial_clicked();

    void on_btnChangeRemarkSerial_clicked();

    void on_btnAppendLM_clicked();

    void on_btnDeleteLM_clicked();

    void on_btnDeleteAllLM_clicked();

    void on_btnChangeRemarkLM_clicked();

    void on_btnDeleteOption_clicked();

    void on_btnDeleteAllOption_clicked();

    void on_btnChangeRemarkOption_clicked();

    void on_btnAppendOption_clicked();

    void on_btnNewConfig_clicked();

    void on_combConfigList_currentIndexChanged(int index);

    void on_btnSaveConfig_clicked();

    void on_btnDeleteConfig_clicked();

    void on_cheManualTemperature_stateChanged(int arg1);

    void on_spinManualTemperature_valueChanged(double arg1);

    void on_spinOutWaterThres_valueChanged(int arg1);

    void on_spinInWaterThres_valueChanged(int arg1);

    void on_actTest_triggered();

    void on_btnSaveResult_clicked();

    void on_btnStartTest_clicked();

private:
    Ui::MainWindow *ui;


    //私有成员
    SerialPortHandler* m_serialPortModel;//串口信息表格
    LMHandler* m_lmModel;//下位机数据模型
    OptionHandler* m_optionModel;//测试数据模型
    QList<ConfigHandler*> m_configList;//配置列表


    //状态栏
    QProgressBar m_timeBar;//轮次进度条
    QProgressBar m_totalBar;//总体进度条
    QLabel m_modeLab;//模式
    QLabel m_currentProgerssLab;//当前进度
    QLabel m_totalPrtgressLab;//总进度


    //测试时使用的私有成员
    QMap<QString,QList<QByteArray>> m_cmd;//建立串口名向指令列表的映射
    QMap<QString,int> m_cmdIndex;//建立串口名向指令列表索引的映射
    QMap<QString,QSerialPort*> m_serialName2serialPtr;//串口名称映射到串口指针
    QMap<QString,OptionHandler*> m_name2project;//建立名称和测试配置之间的映射
    TestHandler m_inWaterMac;//进水电机实例
    TestHandler m_outWaterMac;//回水电机实例
    TestHandler m_temptureMac;//温度计实例
    TestHandler m_esScaleMac;//电子秤实例
    QList<TestHandler*> m_flowMetersMacs;//流量计实例列表
    QList<double>  m_flowMeterValues;//流量计读数
    QList<double> m_startFlowMeterValues;//起始流量计读数
    QList<QStandardItemModel*> m_flowMeterTable;
    double m_esValue;//电子秤读数
    int m_startEsValue;//测试开始时电子秤读数
    double m_currentTemperature;//温度计读数
    TEST_STATE m_testState;//当前测试阶段
    double td[41][10];//水的温度密度对照表
    QTimer timer1;//1号测试驱动定时器
    QTimer timer2;//2号指令驱动定时器

    TEST_STATE m_testMode;//测试模式
    int m_startSpeed;//初始转速
    int m_duration;//时长,用于时长模式
    int m_endSpeed;//终止转速,用于时长模式
    int m_total;//总量,用于总量模式
    int m_testRow;//测试到第几行
    int m_testTimes;//测试到第几次
    int m_preInWaterTime;//预进水时间
    int m_stopWaitTime;//停水等待时间
    int m_currentSpeed;//当前进水电机转速,用于时长模式
    int m_costTime;//进水实际使用时间


    //私有方法
    void init();//程序初始化函数
    bool loadConfig(QString fileName);//加载配置信息
    void saveConfig(QString fileName);//保存配置信息
    void loadConfigFromConfigHandler(ConfigHandler *configStruct);//通过全局配置结构体加载数据
    ConfigHandler* getCurrentConfigFromUI(QString name);//通过当前界面配置构建ConfigHandler对象,并返回其地址
    QString getCurrentDataTime();//获取当前时间,格式为:hh:mm:ss
    void printMsg(QTextBrowser* txtBro,QString msg,MSG_TYPE msgType);//在指定窗口打印指定信息
    bool cheAddressUnique();//检查下位机地址是否唯一
    uint16_t calcCrc16(QByteArray& data, uint16_t count);//计算CRC16校验码
    double getDensity(double temperature);//获取指定温度下的水到的密度,参数为温度
    OptionHandler* copyOptionModel(OptionHandler* model);//负责配置内容到一个新的指针里

    //这部分方法用于测试过程
    bool initTest();//开始测试时初始化各个映射关系,初始化失败返回false,否则返回true
    void testing();//测试驱动函数
    void analysisCmd(QByteArray data);//解析指令所用函数

    //下位机控制函数
    void setInWaterSpeed(int speed);//设置进水泵速度
    void getFlowMeterValues();//更新所有流量计读数
    void setOutWaterState(bool enable);//设置回水电机状态
    void getEsValue();//获取电子秤读数
    void getTemptureValue();//获取温度计读数

protected:
    //保护方法
    void closeEvent(QCloseEvent* e);//重写关闭函数,使程序关闭前保存配置信息

private slots:
    void onTimer1Out();//当一号驱动定时器触发时调用此函数
    void onTimer2Out();//当二号指令驱动定时器触发时调用此函数
    void onReadyRead();//当有串口有数据可读时调用此槽函数
    void on_combFlowMacChoice_currentIndexChanged(int index);
    void on_btnClearResult_clicked();
    void on_actDebug_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_6_clicked();
    void on_btnSaveProject_clicked();
    void on_combChoiceProject_currentTextChanged(const QString &arg1);
    void on_btnDeleteProject_clicked();

};
#endif // MAINWINDOW_H
