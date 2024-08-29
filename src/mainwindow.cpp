#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();//调用程序初始化函数,请勿在此函数前调用任何自定义方法
}

MainWindow::~MainWindow()
{
    delete ui;
}

//程序初始化函数
void MainWindow::init()
{
    this->setCentralWidget(ui->stackedWidget);//设置程序中心组件

    //初始化串口表格数据
    m_serialPortModel = new SerialPortHandler(this);
    ui->tvSerialTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvSerialTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvSerialTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvSerialTable->setModel(m_serialPortModel);

    //初始化下位机数据模型
    m_lmModel = new LMHandler(this);
    ui->tvLM->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvLM->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvLM->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvLM->setModel(m_lmModel);

    //初始化测试方案列表
    m_optionModel = new OptionHandler(this);
//    ui->tvTestOption->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvTestOption->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tvTestOption->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvTestOption->setAlternatingRowColors(true);
    ui->tvTestOption->setPalette(QPalette(QColor(0,206,209)));
    ui->tvTestOption->setModel(m_optionModel);

    //初始化测试结果视图
    ui->tvTest->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvTest->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvTest->setSelectionBehavior(QAbstractItemView::SelectRows);

    //加载配置
    if(!loadConfig(g_configName)){
        QMessageBox::information(0,"提示","未找到配置文件.");
    }

    //初始化成员变量
    m_currentTemperature=g_defaultTemperature;
    m_testState=TEST_STATE::STOP;
    timer1.setInterval(g_timer1Interval);
    timer2.setInterval(g_timer2Interval);
    connect(&timer1,SIGNAL(timeout()),this,SLOT(onTimer1Out()));
    connect(&timer2,SIGNAL(timeout()),this,SLOT(onTimer2Out()));

    //可用性控制
    ui->btnClearResult->hide();
    ui->groupBox_15->setEnabled(false);
    this->showMaximized();

    //状态栏
    QLabel* _lab1=new QLabel(this);
    _lab1->setText("轮次进度:");
    QLabel* _lab2=new QLabel(this);
    _lab2->setText("总体进度:");
    ui->statusbar->addWidget(_lab1);
    m_timeBar.setRange(0,0);
    ui->statusbar->addWidget(&m_timeBar);
    ui->statusbar->addWidget(_lab2);
    m_totalBar.setRange(0,0);
    ui->statusbar->addWidget(&m_totalBar);
    m_modeLab.setText("none");
    ui->statusbar->addWidget(&m_modeLab);
    m_currentProgerssLab.setText("none");//当前进度
    m_totalPrtgressLab.setText("none");//总进度
    ui->statusbar->addWidget(&m_currentProgerssLab);
    ui->statusbar->addWidget(&m_totalPrtgressLab);

    //初始化温度密度对照表
    td[0][0]=999.840;td[0][1]=999.846;td[0][2]=999.853;td[0][3]=999.859;td[0][4]=999.865;td[0][5]=999.871;td[0][6]=999.877;td[0][7]=999.883;td[0][8]=999.888;td[0][9]=999.893;
    td[1][0]=999.898;td[1][1]=999.904;td[1][2]=999.908;td[1][3]=999.913;td[1][4]=999.917;td[1][5]=999.921;td[1][6]=999.925;td[1][7]=999.929;td[1][8]=999.933;td[1][9]=999.937;
    td[2][0]=999.940;td[2][1]=999.943;td[2][2]=999.946;td[2][3]=999.949;td[2][4]=999.952;td[2][5]=999.954;td[2][6]=999.956;td[2][7]=999.959;td[2][8]=999.961;td[2][9]=999.962;
    td[3][0]=999.964;td[3][1]=999.966;td[3][2]=999.967;td[3][3]=999.968;td[3][4]=999.969;td[3][5]=999.970;td[3][6]=999.971;td[3][7]=999.971;td[3][8]=999.972;td[3][9]=999.972;
    td[4][0]=999.972;td[4][1]=999.972;td[4][2]=999.972;td[4][3]=999.971;td[4][4]=999.971;td[4][5]=999.970;td[4][6]=999.969;td[4][7]=999.968;td[4][8]=999.967;td[4][9]=999.965;
    td[5][0]=999.964;td[5][1]=999.962;td[5][2]=999.960;td[5][3]=999.958;td[5][4]=999.956;td[5][5]=999.954;td[5][6]=999.951;td[5][7]=999.949;td[5][8]=999.946;td[5][9]=999.943;
    td[6][0]=999.940;td[6][1]=999.937;td[6][2]=999.934;td[6][3]=999.930;td[6][4]=999.926;td[6][5]=999.923;td[6][6]=999.919;td[6][7]=999.915;td[6][8]=999.910;td[6][9]=999.906;
    td[7][0]=999.901;td[7][1]=999.897;td[7][2]=999.892;td[7][3]=999.887;td[7][4]=999.882;td[7][5]=999.877;td[7][6]=999.871;td[7][7]=999.866;td[7][8]=999.880;td[7][9]=999.854;
    td[8][0]=999.848;td[8][1]=999.842;td[8][2]=999.836;td[8][3]=999.829;td[8][4]=999.823;td[8][5]=999.816;td[8][6]=999.809;td[8][7]=999.802;td[8][8]=999.795;td[8][9]=999.788;
    td[9][0]=999.781;td[9][1]=999.773;td[9][2]=999.765;td[9][3]=999.758;td[9][4]=999.750;td[9][5]=999.742;td[9][6]=999.734;td[9][7]=999.725;td[9][8]=999.717;td[9][9]=999.708;
    td[10][0]=999.699;td[10][1]=999.691;td[10][2]=999.682;td[10][3]=999.672;td[10][4]=999.663;td[10][5]=999.654;td[10][6]=999.644;td[10][7]=999.634;td[10][8]=999.625;td[10][9]=999.615;
    td[11][0]=999.605;td[11][1]=999.595;td[11][2]=999.584;td[11][3]=999.574;td[11][4]=999.563;td[11][5]=999.553;td[11][6]=999.542;td[11][7]=999.531;td[11][8]=999.520;td[11][9]=999.508;
    td[12][0]=999.497;td[12][1]=999.486;td[12][2]=999.474;td[12][3]=999.462;td[12][4]=999.450;td[12][5]=999.439;td[12][6]=999.426;td[12][7]=999.414;td[12][8]=999.402;td[12][9]=999.389;
    td[13][0]=999.377;td[13][1]=999.384;td[13][2]=999.351;td[13][3]=999.338;td[13][4]=999.325;td[13][5]=999.312;td[13][6]=999.299;td[13][7]=999.285;td[13][8]=999.271;td[13][9]=999.258;
    td[14][0]=999.244;td[14][1]=999.230;td[14][2]=999.216;td[14][3]=999.202;td[14][4]=999.187;td[14][5]=999.173;td[14][6]=999.158;td[14][7]=999.144;td[14][8]=999.129;td[14][9]=999.114;
    td[15][0]=999.099;td[15][1]=999.084;td[15][2]=999.069;td[15][3]=999.053;td[15][4]=999.038;td[15][5]=999.022;td[15][6]=999.006;td[15][7]=998.991;td[15][8]=998.975;td[15][9]=998.959;
    td[16][0]=998.943;td[16][1]=998.926;td[16][2]=998.910;td[16][3]=998.893;td[16][4]=998.876;td[16][5]=998.860;td[16][6]=998.843;td[16][7]=998.826;td[16][8]=998.809;td[16][9]=998.792;
    td[17][0]=998.774;td[17][1]=998.757;td[17][2]=998.739;td[17][3]=998.722;td[17][4]=998.704;td[17][5]=998.686;td[17][6]=998.668;td[17][7]=998.650;td[17][8]=998.632;td[17][9]=998.613;
    td[18][0]=998.595;td[18][1]=998.576;td[18][2]=998.557;td[18][3]=998.539;td[18][4]=998.520;td[18][5]=998.501;td[18][6]=998.482;td[18][7]=998.463;td[18][8]=998.443;td[18][9]=998.424;
    td[19][0]=998.404;td[19][1]=998.385;td[19][2]=998.365;td[19][3]=998.345;td[19][4]=998.325;td[19][5]=998.305;td[19][6]=998.285;td[19][7]=998.265;td[19][8]=998.244;td[19][9]=998.224;
    td[20][0]=998.203;td[20][1]=998.182;td[20][2]=998.162;td[20][3]=998.141;td[20][4]=998.120;td[20][5]=998.099;td[20][6]=998.077;td[20][7]=998.056;td[20][8]=998.035;td[20][9]=998.013;
    td[21][0]=997.991;td[21][1]=997.970;td[21][2]=997.948;td[21][3]=997.926;td[21][4]=997.904;td[21][5]=997.882;td[21][6]=997.859;td[21][7]=997.837;td[21][8]=997.815;td[21][9]=997.792;
    td[22][0]=997.769;td[22][1]=997.747;td[22][2]=997.724;td[22][3]=997.701;td[22][4]=997.678;td[22][5]=997.655;td[22][6]=997.631;td[22][7]=997.608;td[22][8]=997.584;td[22][9]=997.561;
    td[23][0]=997.537;td[23][1]=997.513;td[23][2]=997.490;td[23][3]=997.466;td[23][4]=997.442;td[23][5]=997.417;td[23][6]=997.393;td[23][7]=997.396;td[23][8]=997.344;td[23][9]=997.320;
    td[24][0]=997.295;td[24][1]=997.270;td[24][2]=997.246;td[24][3]=997.221;td[24][4]=997.195;td[24][5]=997.170;td[24][6]=997.145;td[24][7]=997.120;td[24][8]=997.094;td[24][9]=997.069;
    td[25][0]=997.043;td[25][1]=997.018;td[25][2]=996.992;td[25][3]=996.966;td[25][4]=996.940;td[25][5]=996.914;td[25][6]=996.888;td[25][7]=996.861;td[25][8]=996.835;td[25][9]=996.809;
    td[26][0]=996.782;td[26][1]=996.755;td[26][2]=996.729;td[26][3]=996.702;td[26][4]=996.675;td[26][5]=996.648;td[26][6]=996.621;td[26][7]=996.594;td[26][8]=996.566;td[26][9]=996.539;
    td[27][0]=996.511;td[27][1]=996.484;td[27][2]=996.456;td[27][3]=996.428;td[27][4]=996.401;td[27][5]=996.373;td[27][6]=996.344;td[27][7]=996.316;td[27][8]=996.288;td[27][9]=996.260;
    td[28][0]=996.231;td[28][1]=996.203;td[28][2]=996.174;td[28][3]=996.146;td[28][4]=996.117;td[28][5]=996.088;td[28][6]=996.059;td[28][7]=996.030;td[28][8]=996.001;td[28][9]=995.972;
    td[29][0]=995.943;td[29][1]=995.913;td[29][2]=995.884;td[29][3]=995.854;td[29][4]=995.825;td[29][5]=995.795;td[29][6]=995.765;td[29][7]=995.753;td[29][8]=995.705;td[29][9]=995.675;
    td[30][0]=995.645;td[30][1]=995.615;td[30][2]=995.584;td[30][3]=995.554;td[30][4]=995.523;td[30][5]=995.493;td[30][6]=995.462;td[30][7]=995.431;td[30][8]=995.401;td[30][9]=995.370;
    td[31][0]=995.339;td[31][1]=995.307;td[31][2]=995.276;td[31][3]=995.245;td[31][4]=995.214;td[31][5]=995.182;td[31][6]=995.151;td[31][7]=995.119;td[31][8]=995.087;td[31][9]=995.055;
    td[32][0]=995.024;td[32][1]=994.992;td[32][2]=994.960;td[32][3]=994.927;td[32][4]=994.895;td[32][5]=994.863;td[32][6]=994.831;td[32][7]=994.798;td[32][8]=994.766;td[32][9]=994.733;
    td[33][0]=994.700;td[33][1]=994.667;td[33][2]=994.635;td[33][3]=994.602;td[33][4]=994.569;td[33][5]=994.535;td[33][6]=994.502;td[33][7]=994.469;td[33][8]=994.436;td[33][9]=994.402;
    td[34][0]=994.369;td[34][1]=994.335;td[34][2]=994.301;td[34][3]=994.267;td[34][4]=994.234;td[34][5]=994.200;td[34][6]=994.166;td[34][7]=994.132;td[34][8]=994.098;td[34][9]=994.063;
    td[35][0]=994.029;td[35][1]=993.994;td[35][2]=993.960;td[35][3]=993.925;td[35][4]=993.891;td[35][5]=993.856;td[35][6]=993.821;td[35][7]=993.786;td[35][8]=993.751;td[35][9]=993.716;
    td[36][0]=993.681;td[36][1]=993.646;td[36][2]=993.610;td[36][3]=993.575;td[36][4]=993.540;td[36][5]=993.504;td[36][6]=993.469;td[36][7]=993.433;td[36][8]=993.397;td[36][9]=993.361;
    td[37][0]=993.325;td[37][1]=993.280;td[37][2]=993.253;td[37][3]=993.217;td[37][4]=993.181;td[37][5]=993.144;td[37][6]=993.108;td[37][7]=993.072;td[37][8]=993.035;td[37][9]=992.999;
    td[38][0]=992.962;td[38][1]=992.925;td[38][2]=992.888;td[38][3]=992.851;td[38][4]=992.814;td[38][5]=992.777;td[38][6]=992.740;td[38][7]=992.703;td[38][8]=992.665;td[38][9]=992.628;
    td[39][0]=992.591;td[39][1]=992.553;td[39][2]=992.516;td[39][3]=992.478;td[39][4]=992.440;td[39][5]=992.402;td[39][6]=992.364;td[39][7]=992.326;td[39][8]=992.288;td[39][9]=992.250;
    td[40][0]=992.212;td[40][1]=991.826;td[40][2]=991.432;td[40][3]=991.031;td[40][4]=990.623;td[40][5]=990.208;td[40][6]=989.786;td[40][7]=987.358;td[40][8]=988.922;td[40][9]=988.479;
}

//加载配置文件,加载成功返回true,失败返回false
bool MainWindow::loadConfig(QString fileName)
{
    QFile file(fileName);
    if(!file.exists()){//如果配置文件不存在则返回false
        return false;
    }

    //清除原有配置列表
    ui->combConfigList->clear();
    m_configList.clear();
    ui->combChoiceProject->clear();
    m_name2project.clear();

    //如果配置文件存在则加载配置文件
    QSettings setting(fileName,QSettings::IniFormat);
    setting.setIniCodec("UTF-8");

    //配置组合数量
    int configCount=0;
    configCount=setting.value("config/count").toInt();

    //当前配置索引
    int configIndex=setting.value("globalSetting/configIndex").toInt();

    QStringList names;

    for(int j=0;j<configCount;++j){
        //全局设置部分
        ConfigHandler* configStruct = new ConfigHandler(this);
        configStruct->configName=setting.value("globalSetting/configName"+QString::number(j)).toString();//当前配置名称
        configStruct->cheManualTemperature=setting.value("globalSetting/cheManualTemperature"+QString::number(j)).toString();
        configStruct->spinManualTemperature=setting.value("globalSetting/spinManualTemperature"+QString::number(j)).toString();
        configStruct->spinOutWaterThres=setting.value("globalSetting/spinOutWaterThres"+QString::number(j)).toString();
        configStruct->spinInWaterThres=setting.value("globalSetting/spinInWaterThres"+QString::number(j)).toString();

        //串口部分
        int serialPortRow=setting.value("serialPort/count"+QString::number(j)).toInt();
        for(int i=0;i<serialPortRow;++i){
            configStruct->serialPortHandler->insertRow(configStruct->serialPortHandler->rowCount());
            int dataRow=configStruct->serialPortHandler->rowCount() - 1;

            QString name="serialPort/name"+QString::number(j)+QString::number(i);
            QString baute="serialPort/baute"+QString::number(j)+QString::number(i);
            QString databit="serialPort/dataBit"+QString::number(j)+QString::number(i);
            QString paritybit="serialPort/parityBit"+QString::number(j)+QString::number(i);
            QString stopbit="serialPort/stopBit"+QString::number(j)+QString::number(i);
            QString flowctrl="serialPort/flowctrl"+QString::number(j)+QString::number(i);
            QString connectstate="serialPort/connectState"+QString::number(j)+QString::number(i);
            QString remark="serialPort/remark"+QString::number(j)+QString::number(i);

            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,0),setting.value(name));
            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,1),setting.value(baute));
            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,2),setting.value(databit));
            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,3),setting.value(paritybit));
            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,4),setting.value(stopbit));
            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,5),setting.value(flowctrl));
            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,6),setting.value(connectstate));
            configStruct->serialPortHandler->setData(configStruct->serialPortHandler->index(dataRow,7),setting.value(remark));
        }

        //下位机部分
        int lmrow=setting.value("lm/count"+QString::number(j)).toInt();
        for(int i=0;i<lmrow;++i){
            QString name="lm/name"+QString::number(j)+QString::number(i);
            QString serialname="lm/serialPortName"+QString::number(j)+QString::number(i);
            QString address="lm/address"+QString::number(j)+QString::number(i);
            QString role="lm/role"+QString::number(j)+QString::number(i);
            QString remark="lm/remark"+QString::number(j)+QString::number(i);

            configStruct->lmHandler->insertRow(configStruct->lmHandler->rowCount());
            int dataRow=configStruct->lmHandler->rowCount() - 1;

            configStruct->lmHandler->setData(configStruct->lmHandler->index(dataRow,0),setting.value(name));
            configStruct->lmHandler->setData(configStruct->lmHandler->index(dataRow,1),setting.value(serialname));
            configStruct->lmHandler->setData(configStruct->lmHandler->index(dataRow,2),setting.value(address));
            configStruct->lmHandler->setData(configStruct->lmHandler->index(dataRow,3),setting.value(role));
            configStruct->lmHandler->setData(configStruct->lmHandler->index(dataRow,4),setting.value(remark));
        }

        //测试配置部分
        int optionrow=setting.value("option/count"+QString::number(j)).toInt();

        for(int i=0;i<optionrow;++i){
            QString name="option/name"+QString::number(j)+QString::number(i);
            QString mode="option/mode"+QString::number(j)+QString::number(i);
            QString startspeed="option/startSpeed"+QString::number(j)+QString::number(i);
            QString duration="option/duration"+QString::number(j)+QString::number(i);
            QString endspeed="option/endSpeed"+QString::number(j)+QString::number(i);
            QString total="option/total"+QString::number(j)+QString::number(i);
            QString times="option/times"+QString::number(j)+QString::number(i);
            QString preinwaterduration="option/preinwaterduration"+QString::number(j)+QString::number(i);
            QString waittime="option/waitTime"+QString::number(j)+QString::number(i);
            QString remark="option/remark"+QString::number(j)+QString::number(i);

            configStruct->optionHandler->insertRow(configStruct->optionHandler->rowCount());
            int dataRow=configStruct->optionHandler->rowCount() - 1;

            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,0),setting.value(name));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,1),setting.value(mode));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,2),setting.value(startspeed));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,3),setting.value(duration));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,4),setting.value(endspeed));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,5),setting.value(total));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,6),setting.value(times));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,7),setting.value(preinwaterduration));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,8),setting.value(waittime));
            configStruct->optionHandler->setData(configStruct->optionHandler->index(dataRow,9),setting.value(remark));
        }

        m_configList.append(configStruct);
        names<<configStruct->configName;
    }

    disconnect(ui->combConfigList,SIGNAL(currentIndexChanged(int)),this,SLOT(on_combConfigList_currentIndexChanged(int)));
    ui->combConfigList->addItems(names);
    connect(ui->combConfigList,SIGNAL(currentIndexChanged(int)),this,SLOT(on_combConfigList_currentIndexChanged(int)));
    if(configIndex!=-1){
        ui->combConfigList->setCurrentIndex(configIndex);
        emit ui->combConfigList->currentIndexChanged(configIndex);
    }

    //测试配置列表部分
    int currentOptionRow=setting.value("optionList/currentIndex").toInt();
    int count=setting.value("optionList/count").toInt();

    for(int i=0;i<count;++i){
        QString optionName=setting.value("optionList/optionName"+QString::number(i)).toString();

        OptionHandler* _model=new OptionHandler(this);
        int row=setting.value("optionList/count"+QString::number(i)).toInt();

        for(int j=0;j<row;++j){
            QString name="optionList/name"+QString::number(i)+QString::number(j);
            QString mode="optionList/mode"+QString::number(i)+QString::number(j);
            QString startspeed="optionList/startSpeed"+QString::number(i)+QString::number(j);
            QString duration="optionList/duration"+QString::number(i)+QString::number(j);
            QString endspeed="optionList/endSpeed"+QString::number(i)+QString::number(j);
            QString total="optionList/total"+QString::number(i)+QString::number(j);
            QString times="optionList/times"+QString::number(i)+QString::number(j);
            QString preinwaterduration="optionList/preinwaterduration"+QString::number(i)+QString::number(j);
            QString waittime="optionList/waitTime"+QString::number(i)+QString::number(j);
            QString remark="optionList/remark"+QString::number(i)+QString::number(j);

            _model->insertRow(_model->rowCount());
            int dataRow=_model->rowCount()-1;

            _model->setData(_model->index(dataRow,0),setting.value(name));
            _model->setData(_model->index(dataRow,1),setting.value(mode));
            _model->setData(_model->index(dataRow,2),setting.value(startspeed));
            _model->setData(_model->index(dataRow,3),setting.value(duration));
            _model->setData(_model->index(dataRow,4),setting.value(endspeed));
            _model->setData(_model->index(dataRow,5),setting.value(total));
            _model->setData(_model->index(dataRow,6),setting.value(times));
            _model->setData(_model->index(dataRow,7),setting.value(preinwaterduration));
            _model->setData(_model->index(dataRow,8),setting.value(waittime));
            _model->setData(_model->index(dataRow,9),setting.value(remark));
        }
        m_name2project[optionName]=_model;
        ui->combChoiceProject->addItem(optionName);

    }
    if(currentOptionRow!=-1){
        ui->combChoiceProject->setCurrentIndex(currentOptionRow);
        emit ui->combChoiceProject->currentIndexChanged(currentOptionRow);
    }

    return true;
}

//保存配置文件
void MainWindow:: saveConfig(QString fileName)
{
    QSettings setting(fileName,QSettings::IniFormat);
    setting.setIniCodec("UTF-8");

    int configRow=m_configList.count();

    //配置组合数量
    setting.setValue("config/count",configRow);
    setting.setValue("globalSetting/configIndex",ui->combConfigList->currentIndex());//当前配置索引

    for(int j=0;j<configRow;++j){

        //全局设置部分
        setting.setValue("globalSetting/configName"+QString::number(j),m_configList.at(j)->configName);//当前配置名称
        setting.setValue("globalSetting/cheManualTemperature"+QString::number(j),m_configList.at(j)->cheManualTemperature);
        setting.setValue("globalSetting/spinManualTemperature"+QString::number(j),m_configList.at(j)->spinManualTemperature);
        setting.setValue("globalSetting/spinOutWaterThres"+QString::number(j),m_configList.at(j)->spinOutWaterThres);
        setting.setValue("globalSetting/spinInWaterThres"+QString::number(j),m_configList.at(j)->spinInWaterThres);

        //串口部分
        int serialPortRow=m_configList.at(j)->serialPortHandler->rowCount();
        setting.setValue("serialPort/count"+QString::number(j),serialPortRow);
        for(int i=0;i<serialPortRow;++i){
            QString name="serialPort/name"+QString::number(j)+QString::number(i);
            QString baute="serialPort/baute"+QString::number(j)+QString::number(i);
            QString databit="serialPort/dataBit"+QString::number(j)+QString::number(i);
            QString paritybit="serialPort/parityBit"+QString::number(j)+QString::number(i);
            QString stopbit="serialPort/stopBit"+QString::number(j)+QString::number(i);
            QString flowctrl="serialPort/flowctrl"+QString::number(j)+QString::number(i);
            QString connectstate="serialPort/connectState"+QString::number(j)+QString::number(i);
            QString remark="serialPort/remark"+QString::number(j)+QString::number(i);

            setting.setValue(name,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,0)));
            setting.setValue(baute,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,1)));
            setting.setValue(databit,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,2)));
            setting.setValue(paritybit,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,3)));
            setting.setValue(stopbit,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,4)));
            setting.setValue(flowctrl,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,5)));
            setting.setValue(connectstate,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,6)));
            setting.setValue(remark,m_configList.at(j)->serialPortHandler->data(m_configList.at(j)->serialPortHandler->index(i,7)));
        }

        //下位机部分
        int lmrow=m_configList.at(j)->lmHandler->rowCount();
        setting.setValue("lm/count"+QString::number(j),lmrow);
        for(int i=0;i<lmrow;++i){
            QString name="lm/name"+QString::number(j)+QString::number(i);
            QString serialname="lm/serialPortName"+QString::number(j)+QString::number(i);
            QString address="lm/address"+QString::number(j)+QString::number(i);
            QString role="lm/role"+QString::number(j)+QString::number(i);
            QString remark="lm/remark"+QString::number(j)+QString::number(i);

            setting.setValue(name,m_configList.at(j)->lmHandler->data(m_configList.at(j)->lmHandler->index(i,0)));
            setting.setValue(serialname,m_configList.at(j)->lmHandler->data(m_configList.at(j)->lmHandler->index(i,1)));
            setting.setValue(address,m_configList.at(j)->lmHandler->data(m_configList.at(j)->lmHandler->index(i,2)));
            setting.setValue(role,m_configList.at(j)->lmHandler->data(m_configList.at(j)->lmHandler->index(i,3)));
            setting.setValue(remark,m_configList.at(j)->lmHandler->data(m_configList.at(j)->lmHandler->index(i,4)));
        }

        //测试配置部分
        int optionrow=m_configList.at(j)->optionHandler->rowCount();
        setting.setValue("option/count"+QString::number(j),optionrow);
        for(int i=0;i<optionrow;++i){
            QString name="option/name"+QString::number(j)+QString::number(i);
            QString mode="option/mode"+QString::number(j)+QString::number(i);
            QString startspeed="option/startSpeed"+QString::number(j)+QString::number(i);
            QString duration="option/duration"+QString::number(j)+QString::number(i);
            QString endspeed="option/endSpeed"+QString::number(j)+QString::number(i);
            QString total="option/total"+QString::number(j)+QString::number(i);
            QString times="option/times"+QString::number(j)+QString::number(i);
            QString preinwaterduration="option/preinwaterduration"+QString::number(j)+QString::number(i);
            QString waittime="option/waitTime"+QString::number(j)+QString::number(i);
            QString remark="option/remark"+QString::number(j)+QString::number(i);

            setting.setValue(name,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,0)));
            setting.setValue(mode,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,1)));
            setting.setValue(startspeed,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,2)));
            setting.setValue(duration,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,3)));
            setting.setValue(endspeed,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,4)));
            setting.setValue(total,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,5)));
            setting.setValue(times,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,6)));
            setting.setValue(preinwaterduration,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,7)));
            setting.setValue(waittime,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,8)));
            setting.setValue(remark,m_configList.at(j)->optionHandler->data(m_configList.at(j)->optionHandler->index(i,9)));
        }
    }

    //测试配置列表部分
    int currentOptionRow=ui->combChoiceProject->currentIndex();
    setting.setValue("optionList/currentIndex",currentOptionRow);
    setting.setValue("optionList/count",ui->combChoiceProject->count());
    for(int i=0;i<ui->combChoiceProject->count();++i){
        QString optionName=ui->combChoiceProject->itemText(i);
        setting.setValue("optionList/optionName"+QString::number(i),optionName);
        OptionHandler* _model=m_name2project[optionName];

        int row=_model->rowCount();
        setting.setValue("optionList/count"+QString::number(i),row);


        for(int j=0;j<row;++j){

            QString name="optionList/name"+QString::number(i)+QString::number(j);
            QString mode="optionList/mode"+QString::number(i)+QString::number(j);
            QString startspeed="optionList/startSpeed"+QString::number(i)+QString::number(j);
            QString duration="optionList/duration"+QString::number(i)+QString::number(j);
            QString endspeed="optionList/endSpeed"+QString::number(i)+QString::number(j);
            QString total="optionList/total"+QString::number(i)+QString::number(j);
            QString times="optionList/times"+QString::number(i)+QString::number(j);
            QString preinwaterduration="optionList/preinwaterduration"+QString::number(i)+QString::number(j);
            QString waittime="optionList/waitTime"+QString::number(i)+QString::number(j);
            QString remark="optionList/remark"+QString::number(i)+QString::number(j);


            setting.setValue(name,_model->data(_model->index(j,0)));
            setting.setValue(mode,_model->data(_model->index(j,1)));
            setting.setValue(startspeed,_model->data(_model->index(j,2)));
            setting.setValue(duration,_model->data(_model->index(j,3)));
            setting.setValue(endspeed,_model->data(_model->index(j,4)));
            setting.setValue(total,_model->data(_model->index(j,5)));
            setting.setValue(times,_model->data(_model->index(j,6)));
            setting.setValue(preinwaterduration,_model->data(_model->index(j,7)));
            setting.setValue(waittime,_model->data(_model->index(j,8)));
            setting.setValue(remark,_model->data(_model->index(j,9)));
        }
    }

}

//通过全局配置结构体加载数据
void MainWindow::loadConfigFromConfigHandler(ConfigHandler* configStruct)
{
    //加载新配置
    ui->cheManualTemperature->setChecked(configStruct->cheManualTemperature.toInt());
    ui->spinManualTemperature->setValue(configStruct->spinManualTemperature.toDouble());
    ui->spinOutWaterThres->setValue(configStruct->spinOutWaterThres.toInt());
    ui->spinInWaterThres->setValue(configStruct->spinInWaterThres.toInt());

    m_serialPortModel=configStruct->serialPortHandler;
    ui->tvSerialTable->setModel(m_serialPortModel);

    m_lmModel=configStruct->lmHandler;
    ui->tvLM->setModel(m_lmModel);

    m_optionModel=configStruct->optionHandler;
    ui->tvTestOption->setModel(m_optionModel);

    m_serialPortModel->updateConnectState();
}

//通过当前界面配置构建ConfigHandler对象,并返回其地址
ConfigHandler *MainWindow::getCurrentConfigFromUI(QString name)
{
    //全局设置部分
    ConfigHandler* configStruct = new ConfigHandler(this);
    configStruct->configName=name;//当前配置名称

    if(ui->cheManualTemperature->isChecked()){
        configStruct->cheManualTemperature="1";
    }else{
        configStruct->cheManualTemperature="0";
    }
    configStruct->spinManualTemperature=QString::number(ui->spinManualTemperature->value());
    configStruct->spinOutWaterThres=QString::number(ui->spinOutWaterThres->value());
    configStruct->spinInWaterThres=QString::number(ui->spinInWaterThres->value());

    configStruct->serialPortHandler=m_serialPortModel;
    configStruct->lmHandler=m_lmModel;
    configStruct->optionHandler=m_optionModel;
    return configStruct;
}

//获取当前时间,格式为:hh:mm:ss
QString MainWindow::getCurrentDataTime()
{
    QString temp;
    temp.append("[");
    temp.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
    temp.append("]");
    return temp;
}

//在指定窗口打印指定信息
void MainWindow::printMsg(QTextBrowser *obj, QString msg, MSG_TYPE type)
{
    //设置文本颜色
    if(type == MSG_TYPE::SYSTEM){//打印系统消息
        obj->setTextColor("black");
    }else if(type == MSG_TYPE::SUCCESS){//打印操作成功消息
        obj->setTextColor("green");
    }else if(type == MSG_TYPE::MSGFALSE){//打印操作失败消息
        obj->setTextColor("red");
    }else if(type == MSG_TYPE::FLOWMAC){//打印操作失败消息
        obj->setTextColor("darkcyan");
    }else if(type == MSG_TYPE::TEMPTURE){//打印操作失败消息
        obj->setTextColor("coral");
    }else if(type == MSG_TYPE::ES){//打印操作失败消息
        obj->setTextColor("royalblue");
    }

    //打印消息
    obj->append(getCurrentDataTime()+msg);

    obj->setTextColor("black");
}

//检查下位机地址是否唯一,如果不唯一,则返回false;如果唯一则返回true
bool MainWindow::cheAddressUnique()
{
    int row=m_lmModel->rowCount();
    QStringList addressList;
    for(int i=0;i<row;++i){
        addressList << m_lmModel->data(m_lmModel->index(i,2)).toString();
    }
    for(int i=row-1;i>=0;--i){
        QString temp=addressList.at(i);
        addressList.removeAt(i);
        if(addressList.contains(temp)){
            return false;
        }
    }
    return true;
}

//计算CRC16校验码,参数一data为需要计算的字节组,参数二count位字节长度
uint16_t MainWindow::calcCrc16(QByteArray& data, uint16_t count)
{
    uint16_t crc = 0xFFFF;
    uint8_t ch;

    for (int i = 0; i < count; i++) {
        ch = (unsigned char )(data[i]);
        crc ^= ch;

        for (int j = 0; j < 8; j++) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

//返回指定温度下的水的密度,参数为温度
double MainWindow::getDensity(double temperature)
{
    if(ui->cheManualTemperature->isChecked()){
        temperature=ui->spinManualTemperature->value();
    }
    if(temperature > 40.9) temperature = 40.9;
    else if(temperature < 0.0)temperature = 0;
    return (td[(int)temperature][(int)((temperature - (int)temperature) * 10)] / 1000);
}

//负责配置内容到一个新的指针里
OptionHandler* MainWindow::copyOptionModel(OptionHandler *model)
{
    OptionHandler* _temp=new OptionHandler(this);
    for(int i=0;i<model->rowCount();++i){
        _temp->insertRow(_temp->rowCount());
        for(int j=0;j<model->columnCount();++j){
            _temp->setData(_temp->index(i,j),model->data(model->index(i,j)));
        }
    }
    return _temp;
}


//开始测试时初始化
bool MainWindow:: initTest()
{

    //清除原有数据
    m_cmd.clear();
    m_serialName2serialPtr.clear();
    m_cmdIndex.clear();
    m_flowMetersMacs.clear();
    m_flowMeterValues.clear();
    ui->combFlowMacChoice->clear();
    m_flowMeterTable.clear();
    m_testRow=0;
    m_testTimes=0;
    m_esValue=-1.5;

    //初始化各个映射关系
    int serialPortRow=m_serialPortModel->rowCount();
    for(int i=0;i<serialPortRow;++i){
        QSerialPort* _serialPort=m_serialPortModel->connectSerialPort(m_serialPortModel->data(m_serialPortModel->index(i,0)).toString(),m_serialPortModel->data(m_serialPortModel->index(i,1)).toString(),
                                                                        m_serialPortModel->data(m_serialPortModel->index(i,2)).toString(),m_serialPortModel->data(m_serialPortModel->index(i,3)).toString(),
                                                                        m_serialPortModel->data(m_serialPortModel->index(i,4)).toString(),m_serialPortModel->data(m_serialPortModel->index(i,5)).toString());
        if(_serialPort==nullptr){
            QMessageBox::warning(0,"警告","存在无法正常打开的串口.");
            return false;
        }
        m_serialName2serialPtr[m_serialPortModel->data(m_serialPortModel->index(i,0)).toString()]=_serialPort;
        m_cmd[m_serialPortModel->data(m_serialPortModel->index(i,0)).toString()];
        m_cmdIndex[m_serialPortModel->data(m_serialPortModel->index(i,0)).toString()]=0;
        connect(_serialPort,SIGNAL(readyRead()),this,SLOT(onReadyRead()));//连接信号槽
    }


    //初始化下位机
    int inWaterMacCount=0;
    int outWaterMacCount=0;
    int esMacCount=0;
    int flowMeterCount=0;

    QStringList flowMacAddresses;
    int lmRow=m_lmModel->rowCount();
    for(int i=0;i<lmRow;++i){
        QString role=m_lmModel->data(m_lmModel->index(i,3)).toString();
        if(role=="松下泵"){
            m_inWaterMac.name=m_lmModel->data(m_lmModel->index(i,0)).toString();
            m_inWaterMac.serialPortName=m_lmModel->data(m_lmModel->index(i,1)).toString();
            m_inWaterMac.address=m_lmModel->data(m_lmModel->index(i,2)).toString();
            m_inWaterMac.cmdList=&m_cmd[m_inWaterMac.serialPortName];
            m_inWaterMac.macType=LMMAC_TYPE::PANPUMP;
            inWaterMacCount++;
        }else if(role=="蠕动泵"){
            m_inWaterMac.name=m_lmModel->data(m_lmModel->index(i,0)).toString();
            m_inWaterMac.serialPortName=m_lmModel->data(m_lmModel->index(i,1)).toString();
            m_inWaterMac.address=m_lmModel->data(m_lmModel->index(i,2)).toString();
            m_inWaterMac.cmdList=&m_cmd[m_inWaterMac.serialPortName];
            m_inWaterMac.macType=LMMAC_TYPE::PERPUMP;
            inWaterMacCount++;
        }else if(role=="回水泵"){
            m_outWaterMac.name=m_lmModel->data(m_lmModel->index(i,0)).toString();
            m_outWaterMac.serialPortName=m_lmModel->data(m_lmModel->index(i,1)).toString();
            m_outWaterMac.address=m_lmModel->data(m_lmModel->index(i,2)).toString();
            m_outWaterMac.cmdList=&m_cmd[m_outWaterMac.serialPortName];
            m_outWaterMac.macType=LMMAC_TYPE::OUTWATERMAC;
            outWaterMacCount++;
        }else if(role=="温度计"){
            m_temptureMac.name=m_lmModel->data(m_lmModel->index(i,0)).toString();
            m_temptureMac.serialPortName=m_lmModel->data(m_lmModel->index(i,1)).toString();
            m_temptureMac.address=m_lmModel->data(m_lmModel->index(i,2)).toString();
            m_temptureMac.cmdList=&m_cmd[m_temptureMac.serialPortName];
            m_temptureMac.macType=LMMAC_TYPE::TEMPRATURE;
        }else if(role=="流量计"){
            TestHandler* _handler=new TestHandler(this);
            _handler->name=m_lmModel->data(m_lmModel->index(i,0)).toString();
            _handler->serialPortName=m_lmModel->data(m_lmModel->index(i,1)).toString();
            _handler->address=m_lmModel->data(m_lmModel->index(i,2)).toString();
            _handler->cmdList=&m_cmd[_handler->serialPortName];
            _handler->macType=LMMAC_TYPE::FLOWMETER;
            m_flowMeterValues.append(-1.5);
            flowMacAddresses.append(_handler->address);
            m_flowMetersMacs.append(_handler);

            //初始化表格信息
            QStandardItemModel* model=new QStandardItemModel(this);
            model->setColumnCount(10);
            model->setHeaderData(0,Qt::Horizontal,"编号");
            model->setHeaderData(1,Qt::Horizontal,"模式");
            model->setHeaderData(2,Qt::Horizontal,"初始转速\n(r/min)");
            model->setHeaderData(3,Qt::Horizontal,"时长\n(s)");
            model->setHeaderData(4,Qt::Horizontal,"终止转速\n(r/min)");
            model->setHeaderData(5,Qt::Horizontal,"标准体积\n(ml)");
            model->setHeaderData(6,Qt::Horizontal,"被测表体积\n(ml)");
            model->setHeaderData(7,Qt::Horizontal,"示值误差\n(%)");
            model->setHeaderData(8,Qt::Horizontal,"流速\n(L/h)");
            model->setHeaderData(9,Qt::Horizontal,"标准器温度\n(°C)");

            m_flowMeterTable.append(model);
            flowMeterCount++;
        }else if(role=="电子秤"){
            m_esScaleMac.name=m_lmModel->data(m_lmModel->index(i,0)).toString();
            m_esScaleMac.serialPortName=m_lmModel->data(m_lmModel->index(i,1)).toString();
            m_esScaleMac.address=m_lmModel->data(m_lmModel->index(i,2)).toString();
            m_esScaleMac.cmdList=&m_cmd[m_esScaleMac.serialPortName];
            m_esScaleMac.macType=LMMAC_TYPE::ESCALE;
            esMacCount++;
        }
    }

    if(inWaterMacCount<1){
        QMessageBox::warning(0,"警告","缺少进水电机.");
        return false;
    }else if(inWaterMacCount>1){
        QMessageBox::warning(0,"警告","进水电机过多.");
        return false;
    }
    if(outWaterMacCount<1){
        QMessageBox::warning(0,"警告","缺少回水电机.");
        return false;
    }else if(outWaterMacCount>1){
        QMessageBox::warning(0,"警告","回水电机过多.");
        return false;
    }
    if(flowMeterCount==0){
        QMessageBox::warning(0,"警告","请至少添加一个流量计.");
        return false;
    }
    if(esMacCount<1){
        QMessageBox::warning(0,"警告","缺少电子秤.");
        return false;
    }else if(esMacCount>1){
        QMessageBox::warning(0,"警告","电子秤过多.");
        return false;
    }


    ui->combFlowMacChoice->addItems(flowMacAddresses);

    //更新信息
    getFlowMeterValues();//获取流量计读数
    getEsValue();//获取电子秤读数
    getTemptureValue();//获取温度计读数

    return true;
}

//测试过程驱动函数
void MainWindow::testing()
{
    //更新信息
    getFlowMeterValues();//获取流量计读数
    getEsValue();//获取电子秤读数
    getTemptureValue();//获取温度计读数

    if(m_esValue >= (ui->spinInWaterThres->value() * 1000)){//如果当前电子秤读数小于水桶下限阈值,则进入预进水阶段
        if(timer1.isActive()){
            ui->btnStartTest->click();
            printMsg(ui->txtLogMain,"已达到最大进水上限,测试停止.",MSG_TYPE::MSGFALSE);
            return;
        }
    }

    if(m_testState==TEST_STATE::STOP){//停止阶段
        if(m_esValue==-1.5){//如果没有读到电子秤读数
            printMsg(ui->txtLogMain,"电子秤尚未就绪.",MSG_TYPE::MSGFALSE);
        }else{//电子秤就绪
            int i=0;
            for(;i<m_flowMeterValues.count()&&m_flowMeterValues.at(i)!=-1.5;++i);
            if(i==m_flowMeterValues.count()){//所有下位机读数已经初始化

                if(m_esValue < (ui->spinOutWaterThres->value() * 1000)){//如果当前电子秤读数小于水桶下限阈值,则进入预进水阶段
//
                    m_testState=TEST_STATE::PREINWATER;
                    if(m_testTimes>0){//这一行还要继续测试
                        if(m_testMode==TEST_STATE::INWATERTOTAL){//总量模式
                            m_preInWaterTime=m_optionModel->data(m_optionModel->index(m_testRow-1,7)).toInt();
                            m_stopWaitTime=m_optionModel->data(m_optionModel->index(m_testRow-1,8)).toInt();

                            for(int i=0;i<m_flowMeterTable.count();++i){
                                m_flowMeterTable.at(i)->insertRow(m_flowMeterTable.at(i)->rowCount());
                                int row=m_flowMeterTable.at(i)->rowCount()-1;
                                m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,0),m_optionModel->data(m_optionModel->index(m_testRow-1,6)).toInt()-m_testTimes+1);

                                m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,1),"总量模式");
                                m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,2),m_startSpeed);
                                if(ui->cheManualTemperature->isChecked())m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),ui->spinManualTemperature->value());
                                else m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),m_currentTemperature);
                            }

                        }else{//时长模式
                            m_duration=m_optionModel->data(m_optionModel->index(m_testRow-1,3)).toInt();
                            m_preInWaterTime=m_optionModel->data(m_optionModel->index(m_testRow-1,7)).toInt();
                            m_stopWaitTime=m_optionModel->data(m_optionModel->index(m_testRow-1,8)).toInt();
                            m_currentSpeed=m_startSpeed;

                            for(int i=0;i<m_flowMeterTable.count();++i){
                                m_flowMeterTable.at(i)->insertRow(m_flowMeterTable.at(i)->rowCount());
                                int row=m_flowMeterTable.at(i)->rowCount()-1;
                                m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,0),m_optionModel->data(m_optionModel->index(m_testRow-1,6)).toInt()-m_testTimes+1);
                                m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,1),"时长模式");
                                m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,2),m_startSpeed);
                                m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,4),m_endSpeed);
                                if(ui->cheManualTemperature->isChecked())m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),ui->spinManualTemperature->value());
                                else m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),m_currentTemperature);
                            }

                        }

                        m_costTime=0;//重置实际进水使用时间

                        m_testTimes--;
                    }else{//进入下一行测试

                        if(m_testRow>0){//总结上一组测试的稳定性和平均误差
                            for(int j=0;j<m_flowMeterTable.count();++j){
                                m_flowMeterTable.at(j)->insertRow(m_flowMeterTable.at(j)->rowCount());

                                int currentRow = m_flowMeterTable.at(j)->rowCount() - 1;
                                m_flowMeterTable.at(j)->setData(m_flowMeterTable.at(j)->index(currentRow,0),"稳定性");
                                int index = m_flowMeterTable.at(j)->rowCount() - 1 -1;
                                double max = m_flowMeterTable.at(j)->data(m_flowMeterTable.at(j)->index(index,7)).toDouble();
                                double min = max;
                                int cnt = 0;
                                double total = 0.0;
                                for(int i = index; i >= 0; --i){
                                    QString index0 = m_flowMeterTable.at(j)->data(m_flowMeterTable.at(j)->index(i,0)).toString();
                                    if(index0 == "稳定性" || index0 == "平均误差" || index0 == "已弃用"){
                                        break;
                                    }
                                    ++cnt;
                                    double cValue = m_flowMeterTable.at(j)->data(m_flowMeterTable.at(j)->index(i,7)).toDouble();
                                    total += cValue;
                                    max <  cValue ?  max = cValue : 0;
                                    min > cValue ? min = cValue : 0 ;
                                }
                                m_flowMeterTable.at(j)->setData(m_flowMeterTable.at(j)->index(currentRow,7),max - min);

                                m_flowMeterTable.at(j)->insertRow(m_flowMeterTable.at(j)->rowCount());

                                int cRow = m_flowMeterTable.at(j)->rowCount() - 1;
                                m_flowMeterTable.at(j)->setData(m_flowMeterTable.at(j)->index(cRow,0),"平均误差");
                                m_flowMeterTable.at(j)->setData(m_flowMeterTable.at(j)->index(cRow,7),total / cnt);
                            }
                        }
                        if(m_testRow<m_optionModel->rowCount()){//正在测试

                            if(m_testRow>0){
                                m_timeBar.setValue(m_timeBar.maximum());
                            }

                            if(m_optionModel->data(m_optionModel->index(m_testRow,1)).toString()=="总量模式"){//这一行测试模式为总量模式
                                m_testMode=TEST_STATE::INWATERTOTAL;
                                m_startSpeed=m_optionModel->data(m_optionModel->index(m_testRow,2)).toInt();
                                m_total=m_optionModel->data(m_optionModel->index(m_testRow,5)).toInt();
                                m_testTimes=m_optionModel->data(m_optionModel->index(m_testRow,6)).toInt();
                                m_preInWaterTime=m_optionModel->data(m_optionModel->index(m_testRow,7)).toInt();
                                m_stopWaitTime=m_optionModel->data(m_optionModel->index(m_testRow,8)).toInt();

                                for(int i=0;i<m_flowMeterTable.count();++i){
                                    m_flowMeterTable.at(i)->insertRow(m_flowMeterTable.at(i)->rowCount());

                                    int row=m_flowMeterTable.at(i)->rowCount()-1;
                                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,0),m_optionModel->data(m_optionModel->index(m_testRow,6)).toInt()-m_testTimes+1);
                                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,1),"总量模式");
                                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,2),m_startSpeed);
                                    if(ui->cheManualTemperature->isChecked())m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),ui->spinManualTemperature->value());
                                    else m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),m_currentTemperature);
                                }

                                m_modeLab.setText("总量模式");


                                m_currentProgerssLab.setText("当前流量:0 ml");//当前进度
                                m_totalPrtgressLab.setText("总流量:"+QString::number(m_total)+" ml");//总进度


                            }else{//这一行测试模式为时长模式
                                m_testMode=TEST_STATE::INWATERTIME;
                                m_startSpeed=m_optionModel->data(m_optionModel->index(m_testRow,2)).toInt();
                                m_duration=m_optionModel->data(m_optionModel->index(m_testRow,3)).toInt();
                                m_endSpeed=m_optionModel->data(m_optionModel->index(m_testRow,4)).toInt();
                                m_testTimes=m_optionModel->data(m_optionModel->index(m_testRow,6)).toInt();
                                m_preInWaterTime=m_optionModel->data(m_optionModel->index(m_testRow,7)).toInt();
                                m_stopWaitTime=m_optionModel->data(m_optionModel->index(m_testRow,8)).toInt();
                                m_currentSpeed=m_startSpeed;

                                for(int i=0;i<m_flowMeterTable.count();++i){
                                    m_flowMeterTable.at(i)->insertRow(m_flowMeterTable.at(i)->rowCount());

                                    int row=m_flowMeterTable.at(i)->rowCount()-1;
                                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,0),m_optionModel->data(m_optionModel->index(m_testRow,6)).toInt()-m_testTimes+1);
                                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,1),"时长模式");
                                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,2),m_startSpeed);
                                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,4),m_endSpeed);
                                    if(ui->cheManualTemperature->isChecked())m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),ui->spinManualTemperature->value());
                                    else m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,9),m_currentTemperature);
                                }

                                m_modeLab.setText("时长模式");

                                m_currentProgerssLab.setText("当前时长:0 s");//当前进度
                                m_totalPrtgressLab.setText("总时长:"+QString::number(m_duration)+" s");//总进度

                            }

                            m_timeBar.setRange(0,m_testTimes);
                            m_timeBar.setValue(0);


                            m_totalBar.setValue(m_testRow+1);

                            m_testRow++;
                            m_costTime=0;//重置实际进水使用时间
//                            m_startFlowMeterValues=m_flowMeterValues;
                            m_testTimes--;
                        }else{//测试完成
                            timer1.stop();
                            timer2.stop();
                            //断开串口连接
                            for(auto k=m_serialName2serialPtr.begin();k!=m_serialName2serialPtr.end();++k){
                                k.value()->close();
                            }

                            printMsg(ui->txtLogMain,"测时完成!",MSG_TYPE::SUCCESS);
                            ui->btnStartTest->setText("开始测试");
                            ui->btnStartTest->setIcon(QIcon(":/imgs/connectBtn/disconnected.png"));

                            m_timeBar.setRange(0,0);
                            m_totalBar.setRange(0,0);
                            m_modeLab.setText("none");
                            m_currentProgerssLab.setText("none");//当前进度
                            m_totalPrtgressLab.setText("none");//总进度
                        }
                    }
                }else{//如果当前电子秤读数大于水桶下限阈值则进入回水阶段
                    m_testState=TEST_STATE::OUTWATER;
//                    setOutWaterState(true);
                }
            }else{
//                printMsg(ui->txtLogMain,"地址为:"+m_flowMetersMacs.at(i)->address+"的流量计尚未就绪.",MSG_TYPE::MSGFALSE);
            }
        }
    }else if(m_testState==TEST_STATE::PREINWATER){//预进水阶段
        if(m_preInWaterTime>0){//未达到预进水时长
            setInWaterSpeed(g_preInWaterSpeed);
            m_preInWaterTime--;
        }else{//已达到预进水时长
            m_startEsValue=m_esValue;
            m_startFlowMeterValues=m_flowMeterValues;
            if(m_testMode==TEST_STATE::INWATERTOTAL){//总量模式
                m_testState=TEST_STATE::INWATERTOTAL;
                setInWaterSpeed(m_startSpeed);
            }else{//时长模式
                m_testState=TEST_STATE::INWATERTIME;
                setInWaterSpeed(m_startSpeed);
            }
        }
    }else if(m_testState==TEST_STATE::INWATERTOTAL){//总量模式
        m_costTime++;
        double volume = (m_esValue - m_startEsValue) / getDensity(m_currentTemperature);
//        double volume = (m_flowMeterValues[0] - m_startFlowMeterValues[0]) / getDensity(m_currentTemperature);

        m_currentProgerssLab.setText("当前流量:"+QString::number((int)volume)+" ml");//当前进度

        if(volume>m_total){//如果当前进水量已达到目标量
            setInWaterSpeed(0);
            m_testState=TEST_STATE::WAIT;
        }
    }else if(m_testState==TEST_STATE::INWATERTIME){//时长模式

        m_costTime++;
        m_duration--;

        m_currentProgerssLab.setText("当前时长:"+QString::number(m_costTime)+" s");//当前进进度

        if(m_duration>0){//时长模式未完成
            m_currentSpeed+=(m_endSpeed-m_startSpeed)/m_optionModel->data(m_optionModel->index(m_testRow-1,3)).toInt();;
            setInWaterSpeed(m_currentSpeed);
        }else{//时长模式已完成
            setInWaterSpeed(0);
            m_testState=TEST_STATE::WAIT;
        }
    }else if(m_testState==TEST_STATE::WAIT){//停水等待阶段
        m_stopWaitTime--;
        if(m_stopWaitTime<=0){//停水时间已达到,开始进行数据处理
            if(m_testMode==TEST_STATE::INWATERTOTAL){//总量模式
                double volume = (m_esValue - m_startEsValue) / getDensity(m_currentTemperature);
                for(int i=0;i<m_flowMeterTable.count();++i){
                    int row=m_flowMeterTable.at(i)->rowCount()-1;
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,3),m_costTime);
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,5),volume);
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,6),m_flowMeterValues.at(i)-m_startFlowMeterValues.at(i));
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,7),((m_flowMeterValues.at(i)-m_startFlowMeterValues.at(i) - volume) / volume) * 100);
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,8),volume / m_costTime * 3.6);
                }
            }else{//时长模式
                double volume = (m_esValue - m_startEsValue) / getDensity(m_currentTemperature);
                for(int i=0;i<m_flowMeterTable.count();++i){
                    int row=m_flowMeterTable.at(i)->rowCount()-1;
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,3),m_costTime);
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,5),volume);
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,6),m_flowMeterValues.at(i)-m_startFlowMeterValues.at(i));
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,7),((m_flowMeterValues.at(i)-m_startFlowMeterValues.at(i) - volume) / volume) * 100);
                    m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(row,8),volume / m_costTime * 3.6);
                }
            }
            m_testState=TEST_STATE::STOP;
            m_timeBar.setValue(m_timeBar.maximum()-m_testTimes);
        }
    }else if(m_testState==TEST_STATE::OUTWATER){//回水阶段
        if(m_esValue < (ui->spinOutWaterThres->value() * 1000)){//如果当前电子秤读数小于水桶下限阈值,则进入预进水阶段
            setOutWaterState(false);
            m_testState=TEST_STATE::STOP;
        }else{//如果当前电子秤读数大于水桶下限阈值则继续回水
            setOutWaterState(true);
        }
    }
}

//解析指令
void MainWindow::analysisCmd(QByteArray data)
{
    if(ui->checkBox->isChecked()){
        printMsg(ui->txtBrowserDebug,"<-"+data.toHex('\t').toUpper(),MSG_TYPE::SYSTEM);
    }

    if(data.size()>1){
        int address=data.left(1).toHex().toInt();

        //首先检查是否是某个流量计的地址
        int i=0;
        for(;i<m_flowMetersMacs.size()&&m_flowMetersMacs.at(i)->address.toInt()!=address;++i);
        if(i<m_flowMetersMacs.size()){//如果是某个流量计的地址
            QByteArray tempArr = data.left(data.length() - 2);
            uint16_t crc = calcCrc16(tempArr,data.length() - 2);
            QByteArray recDataHex = data.toHex();
            if((recDataHex.right(2).toInt(0,16) == (crc >> 8))){//校验
                if((recDataHex.mid(recDataHex.length() - 4,2).toInt(0,16) == (crc & 0xFF))){
                    QByteArray reaultStr = recDataHex.mid(6,8);
                    m_flowMeterValues[i]=(int32_t)reaultStr.toLongLong(0,16);
                    if(ui->checkBox_3->isChecked()){
                        printMsg(ui->txtBrowserDebug,"解析指令为:流量计读数:"+QString::number((int32_t)reaultStr.toLongLong(0,16)),MSG_TYPE::FLOWMAC);
                    }
                }
            }
        }else if(address==m_esScaleMac.address.toInt()){//如果是电子秤的地址
            QByteArray tempArr = data.left(data.length() - 2);
            uint16_t crc = calcCrc16(tempArr,data.length() - 2);
            QByteArray recDataHex = data.toHex();
            if((recDataHex.right(2).toInt(0,16) == (crc >> 8))){//校验

                if((recDataHex.mid(recDataHex.length() - 4,2).toInt(0,16) == (crc & 0xFF))){

                    //                        if(recDataHex.mid(6,1).toInt(0,16) >= 8){//如果电子秤读数为负
                    //                                    resetESValue();
                    //                                    get_EleScale_Value();
                    //                                    return;
                    //                        }
                    QString esValue;
                    esValue.append(recDataHex.mid(6,4));
                    m_esValue = esValue.toInt(0,16);
                    if(ui->checkBox_4->isChecked()){
                        printMsg(ui->txtBrowserDebug,"解析指令为:电子秤读数为:"+QString::number(m_esValue),MSG_TYPE::ES);
                    }
                }
            }

        }else if(address==m_temptureMac.address.toInt()){//如果是温度计的地址
            QByteArray tempArr = data.left(data.length() - 2);
            uint16_t crc = calcCrc16(tempArr,data.length() - 2);
            QByteArray recDataHex = data.toHex();
            if((recDataHex.right(2).toInt(0,16) == (crc >> 8))){//校验
                if((recDataHex.mid(recDataHex.length() - 4,2).toInt(0,16) == (crc & 0xFF))){
                    QString temptureValue;
                    temptureValue.append(recDataHex.mid(6,4));
                    m_currentTemperature = temptureValue.toInt(0,16) * 0.1;
                    if(ui->checkBox_5->isChecked()){
                        printMsg(ui->txtBrowserDebug,"解析指令为:温度计读数为:"+QString::number(m_currentTemperature)+"°C  对应液体密度为(g/cm³):"+QString::number(getDensity(temptureValue.toInt(0,16) * 0.1)),MSG_TYPE::TEMPTURE);
                    }
                }
            }
        }
    }
}

//设置下位机速度
void MainWindow::setInWaterSpeed(int speed)
{
    if(m_inWaterMac.macType==LMMAC_TYPE::PANPUMP){//松下泵作为进水电机
        QByteArray arr;
        arr.resize(13);
        arr[0] = m_inWaterMac.address.toInt();
        arr[1] = 0x10;
        arr[2] = 0x33;
        arr[3] = 0x08;
        arr[4] = 0x00;
        arr[5] = 0x02;
        arr[6] = 0x04;
        arr[7] = (speed & 0xffff) >> 8;
        arr[8] = speed & 0xff;
        arr[9] = 0x00;
        arr[10] = 0x00;
        uint16_t crc = calcCrc16(arr,11);
        arr[11] = crc & 0xFF;
        arr[12] = crc >> 8;

        //添加指令
        m_inWaterMac.cmdList->append(arr);
    }else if(m_inWaterMac.macType==LMMAC_TYPE::PERPUMP){//蠕动泵最为进水电机
        //正向转速指令
        QByteArray arr;
        arr.resize(8);
        arr[0] = m_inWaterMac.address.toInt();
        arr[1] = 0x06;
        arr[2] = 0x00;
        arr[3] = 0x01;
        if(speed < 0){//反向转速
            arr[3] = 0x02;
        }
        speed = qAbs(speed);
        arr[4] = (speed & 0xffff) >> 8;
        arr[5] = speed & 0xff;
        if(speed == 0){//如果转速为0,即停止蠕动泵
            arr[3] = 0x00;
            arr[4] = 0x00;
            arr[5] = 0x01;
        }
        uint16_t crc = calcCrc16(arr,6);
        arr[6] = crc & 0xFF;
        arr[7] = crc >> 8;

        //添加指令
        m_inWaterMac.cmdList->append(arr);
    }
}

//更新所有流量计读数
void MainWindow::getFlowMeterValues()
{
    int flowMeterCount=m_flowMetersMacs.count();
    for(int i=0;i<flowMeterCount;++i){

        QByteArray arr;
        arr.resize(8);
        arr[0] = m_flowMetersMacs.at(i)->address.toInt();
        arr[1] = 0x03;
        arr[2] = 0x00;
        arr[3] = 0x01;
        arr[4] = 0x00;
        arr[5] = 0x02;
        uint16_t crc = calcCrc16(arr,6);
        arr[6] = crc & 0xFF;
        arr[7] = crc >> 8;

        m_flowMetersMacs.at(i)->cmdList->append(arr);
    }
}

//设置回水电机转速
void MainWindow::setOutWaterState(bool enable)
{
    QByteArray arr;
    arr.resize(10);
    arr[0] = m_outWaterMac.address.toInt();
    arr[1] = 0x0F;
    arr[2] = 0x00;
    arr[3] = 0x00;
    arr[4] = 0x00;
    arr[5] = 0x08;
    arr[6] = 0x01;
    if(enable)arr[7] = 0xFF;
    else arr[7] = 0x00;

    uint16_t crc = calcCrc16(arr,8);
    arr[8] = crc & 0xFF;
    arr[9] = crc >> 8;

    m_outWaterMac.cmdList->append(arr);
}

//获取电子秤读数
void MainWindow::getEsValue()
{
    QByteArray arr;
    arr.resize(8);
    arr[0] = m_esScaleMac.address.toInt();
    arr[1] = 0x03;
    arr[2] = 0x00;
    arr[3] = 0x00;
    arr[4] = 0x00;
    arr[5] = 0x01;
    uint16_t crc = calcCrc16(arr,6);
    arr[6] = crc & 0xFF;
    arr[7] = crc >> 8;

    m_esScaleMac.cmdList->append(arr);
}

//获取温度计读数
void MainWindow::getTemptureValue()
{
    if(m_temptureMac.name.isEmpty())return;
    QByteArray arr;
    arr.resize(8);
    arr[0] = m_temptureMac.address.toInt();
    arr[1] = 0x03;
    arr[2] = 0x00;
    arr[3] = 0x00;
    arr[4] = 0x00;
    arr[5] = 0x01;
    uint16_t crc = calcCrc16(arr,6);
    arr[6] = crc & 0xFF;
    arr[7] = crc >> 8;

    m_temptureMac.cmdList->append(arr);
}

//重写关闭函数,使程序关闭前保存配置信息
void MainWindow::closeEvent(QCloseEvent *e)
{
    saveConfig(g_configName);
    if(timer1.isActive()){

        ui->btnStartTest->click();
    }
    e->accept();
}

//当一号定时器触发时调用此槽函数
void MainWindow::onTimer1Out()
{
    testing();
}

//当二号指令驱动定时器触发时调用此函数
void MainWindow::onTimer2Out()
{
    for(auto i=m_cmd.begin();i!=m_cmd.end();++i){
        QString name=i.key();
        if(m_cmdIndex[name]<i.value().count()){
            m_serialName2serialPtr[name]->write(i.value().at(m_cmdIndex[name]));
            if(ui->checkBox_2->isChecked()){
                printMsg(ui->txtBrowserDebug,"->"+i.value().at(m_cmdIndex[name]).toHex('\t').toUpper(),MSG_TYPE::SYSTEM);
            }
            ++m_cmdIndex[name];
        }
    }
}

//当有串口有数据可读时调用此槽函数
void MainWindow::onReadyRead()
{
    QSerialPort* serialPort=static_cast<QSerialPort*>(sender());
    analysisCmd(serialPort->readAll());
}

//添加串口信息
void MainWindow::on_btnAppendSerial_clicked()
{
    m_serialPortModel->appendSerialInfo();
}

//删除串口信息
void MainWindow::on_btnDeleteSerial_clicked()
{
    //检测当前行是否可用,如果可用就删除选中的一行
    if(ui->tvSerialTable->currentIndex().isValid()){
        m_serialPortModel->removeRow(ui->tvSerialTable->currentIndex().row());
        ui->tvSerialTable->setCurrentIndex(QModelIndex());//清除选中效果,避免意外删除
    }
}

void MainWindow::on_actConfig_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pagConfig);
}


void MainWindow::on_btnCheckConnectSerial_clicked()
{
    m_serialPortModel->updateConnectState();
}


void MainWindow::on_btnDeleteAllSerial_clicked()
{
    //如果当前没有串口信息则退出函数
    if(m_serialPortModel->rowCount()==0){
        return;
    }

    //删除所有串口信息前回先询问用户确定操作
    int result=QMessageBox::question(0,"确认操作","确定要删除所有串口信息吗?");
    if(result==QMessageBox::Yes){
        m_serialPortModel->initTable();
    }
}


void MainWindow::on_btnChangeRemarkSerial_clicked()
{
    //检测当前行是否可用,如果可用调出备注输入框
    if(ui->tvSerialTable->currentIndex().isValid()){
        bool ok;
        QString input=QInputDialog::getText(0,"修改备注","请输入新备注:",QLineEdit::Normal,m_serialPortModel->data(m_serialPortModel->index(ui->tvSerialTable->currentIndex().row(),7)).toString(),&ok);
        if(ok){
            m_serialPortModel->setData(m_serialPortModel->index(ui->tvSerialTable->currentIndex().row(),7),input);
            ui->tvSerialTable->setCurrentIndex(QModelIndex());//清除选中效果,避免意外删除
        }
    }
}


void MainWindow::on_btnAppendLM_clicked()
{
    QStringList serialPorts;
    int row=m_serialPortModel->rowCount();
    for(int i=0;i<row;++i){
        serialPorts << m_serialPortModel->data(m_serialPortModel->index(i,0)).toString();
    }
    m_lmModel->appendLMWindow(serialPorts);
}


void MainWindow::on_btnDeleteLM_clicked()
{
    //检测当前行是否可用,如果可用就删除选中的一行
    if(ui->tvLM->currentIndex().isValid()){
        m_lmModel->removeRow(ui->tvLM->currentIndex().row());
        ui->tvLM->setCurrentIndex(QModelIndex());//清除选中效果,避免意外删除
    }
}


void MainWindow::on_btnDeleteAllLM_clicked()
{
    //如果当前没有串口信息则退出函数
    if(m_lmModel->rowCount()==0){
        return;
    }

    //删除所有串口信息前回先询问用户确定操作
    int result=QMessageBox::question(0,"确认操作","确定要删除所有下位机信息吗?");
    if(result==QMessageBox::Yes){
        m_lmModel->initTable();
    }
}


void MainWindow::on_btnChangeRemarkLM_clicked()
{
    //检测当前行是否可用,如果可用调出备注输入框
    if(ui->tvLM->currentIndex().isValid()){
        bool ok;
        QString input=QInputDialog::getText(0,"修改备注","请输入新备注:",QLineEdit::Normal,m_lmModel->data(m_lmModel->index(ui->tvLM->currentIndex().row(),4)).toString(),&ok);
        if(ok){
            m_lmModel->setData(m_lmModel->index(ui->tvLM->currentIndex().row(),4),input);
            ui->tvLM->setCurrentIndex(QModelIndex());//清除选中效果,避免意外删除
        }
    }
}


void MainWindow::on_btnDeleteOption_clicked()
{
    if(ui->tvTestOption->currentIndex().isValid()){
        m_optionModel->removeRow(ui->tvTestOption->currentIndex().row());
        ui->tvTestOption->setCurrentIndex(QModelIndex());
    }
}


void MainWindow::on_btnDeleteAllOption_clicked()
{
    //如果当前没有串口信息则退出函数
    if(m_optionModel->rowCount()==0){
        return;
    }

    //删除所有串口信息前回先询问用户确定操作
    int result=QMessageBox::question(0,"确认操作","确定要删除所有测试配置信息吗?");
    if(result==QMessageBox::Yes){
        m_optionModel->initTable();
    }
}


void MainWindow::on_btnChangeRemarkOption_clicked()
{
    //检测当前行是否可用,如果可用调出备注输入框
    if(ui->tvTestOption->currentIndex().isValid()){
        bool ok;
        QString input=QInputDialog::getText(0,"修改备注","请输入新备注:",QLineEdit::Normal,m_optionModel->data(m_optionModel->index(ui->tvTestOption->currentIndex().row(),9)).toString(),&ok);
        if(ok){
            m_optionModel->setData(m_optionModel->index(ui->tvTestOption->currentIndex().row(),9),input);
            ui->tvTestOption->setCurrentIndex(QModelIndex());//清除选中效果,避免意外删除
        }
    }
}


void MainWindow::on_btnAppendOption_clicked()
{
    m_optionModel->openAppendOptionWindow();
}


void MainWindow::on_btnNewConfig_clicked()
{
    ui->combConfigList->setEnabled(false);
    ui->btnNewConfig->setEnabled(false);
    ui->btnSaveConfig->setEnabled(true);
    ui->btnDeleteConfig->setText("取消新建配置");

    m_serialPortModel=new SerialPortHandler(this);
    ui->tvSerialTable->setModel(m_serialPortModel);

    m_lmModel= new LMHandler(this);
    ui->tvLM->setModel(m_lmModel);

    m_optionModel=new OptionHandler(this);
    ui->tvTestOption->setModel(m_optionModel);
}


void MainWindow::on_combConfigList_currentIndexChanged(int index)
{
    if(index!=-1){
        loadConfigFromConfigHandler(m_configList.at(index));
    }else{
        m_serialPortModel=new SerialPortHandler(this);
        ui->tvSerialTable->setModel(m_serialPortModel);

        m_lmModel=new LMHandler(this);
        ui->tvLM->setModel(m_lmModel);

        m_optionModel=new OptionHandler(this);
        ui->tvTestOption->setModel(m_optionModel);
    }
}


void MainWindow::on_btnSaveConfig_clicked()
{
    bool ok;
    QString name=QInputDialog::getText(0,"配置名称","请输入配置名称:",QLineEdit::Normal,"",&ok);
    if(ok){
        m_configList.append(getCurrentConfigFromUI(name));
        ui->combConfigList->addItem(name);
        ui->combConfigList->setEnabled(true);
        ui->btnNewConfig->setEnabled(true);
        ui->btnSaveConfig->setEnabled(false);
        ui->combConfigList->setCurrentIndex(ui->combConfigList->count()-1);
        ui->btnDeleteConfig->setText("删除配置");
    }
}


void MainWindow::on_btnDeleteConfig_clicked()
{
    if(ui->combConfigList->isEnabled()){//删除原有配置
        int row=ui->combConfigList->currentIndex();
        if(row!=-1){
            int result=QMessageBox::question(0,"询问","确定要删除配置:"+ui->combConfigList->currentText()+" ?");
            if(result==QMessageBox::Yes){
                m_configList.removeAt(row);
                ui->combConfigList->removeItem(row);
            }
        }
    }else{//删除新建配置
        emit ui->combConfigList->currentIndexChanged(ui->combConfigList->currentIndex());
        ui->combConfigList->setEnabled(true);
        ui->btnNewConfig->setEnabled(true);
        ui->btnSaveConfig->setEnabled(false);
        ui->btnDeleteConfig->setText("删除配置");
    }
}



void MainWindow::on_cheManualTemperature_stateChanged(int arg1)
{
    int index=ui->combConfigList->currentIndex();
    if(index!=-1&&ui->combConfigList->isEnabled()){
        m_configList.at(index)->cheManualTemperature=QString::number(arg1);
    }
}


void MainWindow::on_spinManualTemperature_valueChanged(double arg1)
{
    int index=ui->combConfigList->currentIndex();
    if(index!=-1&&ui->combConfigList->isEnabled()){
        m_configList.at(index)->spinManualTemperature=QString::number(arg1);
    }
}


void MainWindow::on_spinOutWaterThres_valueChanged(int arg1)
{
    int index=ui->combConfigList->currentIndex();
    if(index!=-1&&ui->combConfigList->isEnabled()){
        m_configList.at(index)->spinOutWaterThres=QString::number(arg1);
    }
}


void MainWindow::on_spinInWaterThres_valueChanged(int arg1)
{
    int index=ui->combConfigList->currentIndex();
    if(index!=-1&&ui->combConfigList->isEnabled()){
        m_configList.at(index)->spinInWaterThres=QString::number(arg1);
    }
}


void MainWindow::on_actTest_triggered()
{
    if(!ui->combConfigList->isEnabled()){//如果当正在新建配置,则询问是否保存配置
        QMessageBox::warning(0,"数据保存","请先保存或删除正在编辑的新配置再离开当前页面");
        return;
    }
    ui->stackedWidget->setCurrentWidget(ui->pagTest);
}


void MainWindow::on_btnSaveResult_clicked()
{
    QString fileDir = QCoreApplication::applicationDirPath() + "/" + g_resultSaveDirName;
    QDir dir(fileDir);
    if(!dir.exists()){
        dir.mkdir(fileDir);
    }
    QString filePath = fileDir + "/" +  QDateTime::currentDateTime().toString("yyyy-MM-dd--hh.mm.ss");
    QDir dir2(filePath);
    if(!dir2.exists()){
        dir2.mkdir(filePath);
    }

    QXlsx::Format formatHead;
    formatHead.setFontBold(true);
    formatHead.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    formatHead.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    formatHead.setBorderStyle(QXlsx::Format::BorderThin);

    QXlsx::Format formatCell;
    formatCell.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    formatCell.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    formatCell.setBorderStyle(QXlsx::Format::BorderThin);


    for(int t=0;t<m_flowMeterTable.count();++t){
        QString fileName=filePath+"/"+m_flowMetersMacs.at(t)->address+".xlsx";
        QXlsx::Document m_doc(fileName);
        m_doc.addSheet(m_flowMetersMacs.at(t)->address);
        m_doc.selectSheet(m_flowMetersMacs.at(t)->address);
        int rows=m_flowMeterTable.at(t)->rowCount();
//        int cols=m_flowMeterTable.at(i)->columnCount();

            m_doc.write(1,1,"编号",formatHead);
            m_doc.write(1,2,"模式",formatHead);
            m_doc.write(1,3,QString("初始转速\n(r/min)"),formatHead);
            m_doc.write(1,4,"时长\n(s)",formatHead);
            m_doc.write(1,5,"终止转速\n(r/min)",formatHead);
            m_doc.write(1,6,"标准体积\n(ml)",formatHead);
            m_doc.write(1,7,"被测表体积\n(ml)",formatHead);
            m_doc.write(1,8,"示值误差\n(%)",formatHead);
            m_doc.write(1,9,"平均误差(%)",formatHead);
            m_doc.write(1,10,"稳定性(%)",formatHead);
            m_doc.write(1,11,"流速\n(L/h)",formatHead);
            m_doc.write(1,12,"标准器温度\n(°C)",formatHead);



        for(int i=1;i<=12;i++){
            m_doc.setColumnWidth(i,15);
        }
            int headRow = -1;
            int count = 0;
            double wdx = -1;
            double pjwc = -1;
            int excelRow = 0;
            for (int j = 0;j < rows;++j)
            {
                QString index0 = m_flowMeterTable.at(t)->data(m_flowMeterTable.at(t)->index(j,0)).toString();
                if(index0 == "稳定性"){
                    wdx = m_flowMeterTable.at(t)->data(m_flowMeterTable.at(t)->index(j,7)).toDouble();

                }else if(index0 == "平均误差"){
                           if(headRow == -1){
                                continue;
                           }
                           pjwc = m_flowMeterTable.at(t)->data(m_flowMeterTable.at(t)->index(j,7)).toDouble();

                           m_doc.write(headRow + 1, 9,pjwc,formatCell);
                           m_doc.write(headRow + 1, 10,wdx,formatCell);

                           m_doc.mergeCells(QXlsx::CellRange(headRow + 1,9,headRow + 1 + count,9),formatCell);
                           m_doc.mergeCells(QXlsx::CellRange(headRow + 1,10,headRow + 1 + count,10),formatCell);

                           headRow = -1;
                           count = 0;
                           wdx = -1;
                           pjwc = -1;
                }else if(index0.toInt() == 1){
                            excelRow++;
                           if(headRow == -1){
                                headRow  = excelRow;
                           }
                }else if(index0 == "已弃用"){
                           headRow = -1;
                            ++excelRow;
                           count = 0;
                        continue;
                }else{
                        ++count;
                         excelRow++;

                }
            }

            excelRow = 0;
                int _rows = rows;
                for (int k = 0; k < _rows; ++k)
                {
                                QString _index0 = m_flowMeterTable.at(t)->data(m_flowMeterTable.at(t)->index(k,0)).toString();
                                if(_index0 == "平均误差" || _index0 == "稳定性"){
                                    continue;
                                }

                                excelRow++;
                        for(int j = 0; j < 8 ;++j){
                                    m_doc.write(excelRow + 1, j + 1,m_flowMeterTable.at(t)->data(m_flowMeterTable.at(t)->index(k,j)),formatCell);
                        }

                        m_doc.write(excelRow+ 1, 11,m_flowMeterTable.at(t)->data(m_flowMeterTable.at(t)->index(k,8)),formatCell);
                        m_doc.write(excelRow+ 1, 12,m_flowMeterTable.at(t)->data(m_flowMeterTable.at(t)->index(k,9)),formatCell);
                }


        m_doc.saveAs(fileName);
    }
    printMsg(ui->txtLogMain,"测试结果已保存至:"+filePath,MSG_TYPE::SUCCESS);
}


void MainWindow::on_btnStartTest_clicked()
{
    printMsg(ui->txtLogMain,"尝试启动测试.",MSG_TYPE::SYSTEM);

    if(!timer1.isActive()){//测试停止
        //检测下位机地址是否唯一
        if(!cheAddressUnique()){
            QMessageBox::warning(0,"警告","下位机地址不唯一,启动测试失败.");
            printMsg(ui->txtLogMain,"启动测试失败.",MSG_TYPE::MSGFALSE);
            return;
        }

        //断开串口连接
        for(auto k=m_serialName2serialPtr.begin();k!=m_serialName2serialPtr.end();++k){
            k.value()->close();
        }

        //检查串口连通性`
        m_serialPortModel->updateConnectState();
        int serialPortRow=m_serialPortModel->rowCount();
        for(int i=0;i<serialPortRow;++i){
            if(m_serialPortModel->data(m_serialPortModel->index(i,6)).toString()=="失败"){
                QMessageBox::warning(0,"警告","存在无法正常打开的串口.");
                printMsg(ui->txtLogMain,"启动测试失败.",MSG_TYPE::MSGFALSE);
                return;
            }
        }

        //初始化测试
        if(!initTest()){//如果初始化失败
            for(auto k=m_serialName2serialPtr.begin();k!=m_serialName2serialPtr.end();++k){
                k.value()->close();
            }
            printMsg(ui->txtLogMain,"启动测试失败.",MSG_TYPE::MSGFALSE);
            return;
        }

        //重置测试阶段
        m_testState=TEST_STATE::STOP;

        //启动指令驱动定时器
        timer2.start();

        //启动测试驱动器
        timer1.start();

        ui->btnStartTest->setText("停止测试");
        ui->btnStartTest->setIcon(QIcon(":/imgs/connectBtn/connected.png"));

        m_totalBar.setMaximum(m_optionModel->rowCount());
        m_totalBar.setValue(0);

        printMsg(ui->txtLogMain,"开始测试.",MSG_TYPE::SUCCESS);
    }else{//正在测试
        printMsg(ui->txtLogMain,"测试已停止.",MSG_TYPE::MSGFALSE);
        timer1.stop();
        setInWaterSpeed(0);
        setOutWaterState(false);
//        timer2.stop();


        for(int i=0;i<m_flowMeterTable.count();++i){
            if(m_flowMeterTable.at(i)->rowCount()==0)break;
            m_flowMeterTable.at(i)->setData(m_flowMeterTable.at(i)->index(m_flowMeterTable.at(i)->rowCount()-1,0),"已弃用");
        }

        ui->btnStartTest->setText("开始测试");
        ui->btnStartTest->setIcon(QIcon(":/imgs/connectBtn/disconnected.png"));
//        //断开串口连接
//        for(auto k=m_serialName2serialPtr.begin();k!=m_serialName2serialPtr.end();++k){
//            k.value()->close();
//        }

        m_timeBar.setRange(0,0);
        m_totalBar.setRange(0,0);
        m_modeLab.setText("none");
        m_currentProgerssLab.setText("none");//当前进度
        m_totalPrtgressLab.setText("none");//总进度
    }
}


void MainWindow::on_combFlowMacChoice_currentIndexChanged(int index)
{
    if(index!=-1){
        ui->tvTest->setModel(m_flowMeterTable.at(index));
    }
}


void MainWindow::on_btnClearResult_clicked()
{
    for(int i=0;i<m_flowMeterTable.count();++i){
        m_flowMeterTable.at(i)->clear();

        m_flowMeterTable.at(i)->setColumnCount(10);
        m_flowMeterTable.at(i)->setHeaderData(0,Qt::Horizontal,"编号");
        m_flowMeterTable.at(i)->setHeaderData(1,Qt::Horizontal,"模式");
        m_flowMeterTable.at(i)->setHeaderData(2,Qt::Horizontal,"初始转速\n(r/min)");
        m_flowMeterTable.at(i)->setHeaderData(3,Qt::Horizontal,"时长\n(s)");
        m_flowMeterTable.at(i)->setHeaderData(4,Qt::Horizontal,"终止转速\n(r/min)");
        m_flowMeterTable.at(i)->setHeaderData(5,Qt::Horizontal,"标准体积\n(ml)");
        m_flowMeterTable.at(i)->setHeaderData(6,Qt::Horizontal,"被测表体积\n(ml)");
        m_flowMeterTable.at(i)->setHeaderData(7,Qt::Horizontal,"示值误差\n(%)");
        m_flowMeterTable.at(i)->setHeaderData(8,Qt::Horizontal,"流速\n(L/h)");
        m_flowMeterTable.at(i)->setHeaderData(9,Qt::Horizontal,"标准器温度\n(°C)");
    }

    printMsg(ui->txtLogMain,"已清空测试结果.",MSG_TYPE::SUCCESS);
}


void MainWindow::on_actDebug_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pagDebug);
}


void MainWindow::on_pushButton_clicked()
{

    //断开串口连接
    for(auto k=m_serialName2serialPtr.begin();k!=m_serialName2serialPtr.end();++k){
        k.value()->close();
    }

        //检测下位机地址是否唯一
        if(!cheAddressUnique()){
            QMessageBox::warning(0,"警告","下位机地址不唯一,启动测试失败.");
            printMsg(ui->txtBrowserDebug,"初始化失败.",MSG_TYPE::MSGFALSE);
            return;
        }

        //检查串口连通性`
        m_serialPortModel->updateConnectState();
        int serialPortRow=m_serialPortModel->rowCount();
        for(int i=0;i<serialPortRow;++i){
            if(m_serialPortModel->data(m_serialPortModel->index(i,6)).toString()=="失败"){
                QMessageBox::warning(0,"警告","存在无法正常打开的串口.");
                printMsg(ui->txtBrowserDebug,"初始化失败.",MSG_TYPE::MSGFALSE);
                return;
            }
        }

        //初始化测试
        if(!initTest()){//如果初始化失败
            for(auto k=m_serialName2serialPtr.begin();k!=m_serialName2serialPtr.end();++k){
                k.value()->close();
            }
            printMsg(ui->txtBrowserDebug,"初始化失败.",MSG_TYPE::MSGFALSE);
            return;
        }

        //启动指令驱动定时器
        timer2.start();

        ui->groupBox_15->setEnabled(true);
        printMsg(ui->txtBrowserDebug,"初始化完成.",MSG_TYPE::SUCCESS);
}


void MainWindow::on_pushButton_2_clicked()
{

    setOutWaterState(true);

}


void MainWindow::on_pushButton_3_clicked()
{
    setOutWaterState(false);
}


void MainWindow::on_pushButton_4_clicked()
{
    setInWaterSpeed(ui->spinInWaterSpeed->value());
}

void MainWindow::on_pushButton_5_clicked()
{
    setInWaterSpeed(0);
}


void MainWindow::on_pushButton_8_clicked()
{
    getEsValue();
}


void MainWindow::on_pushButton_9_clicked()
{
    getFlowMeterValues();
}


void MainWindow::on_pushButton_6_clicked()
{
    getTemptureValue();
}


void MainWindow::on_btnSaveProject_clicked()
{
    bool ok;
    QString _name=QInputDialog::getText(0,"输入名称","请输入测试配置名称:",QLineEdit::Normal,"",&ok);
    if(ok){
        m_name2project[_name]=copyOptionModel(static_cast<OptionHandler*>(ui->tvTestOption->model()));
        ui->combChoiceProject->addItem(_name);
        ui->combChoiceProject->setCurrentIndex(ui->combChoiceProject->count()-1);
    }
}


void MainWindow::on_combChoiceProject_currentTextChanged(const QString &arg1)
{
    if(ui->combChoiceProject->currentIndex()!=-1){
        OptionHandler* target=m_name2project[arg1];
        OptionHandler* model=static_cast<OptionHandler*>(ui->tvTestOption->model());
        model->initTable();
        for(int i=0;i<target->rowCount();++i){
                model->insertRow(model->rowCount());
                for(int j=0;j<target->columnCount();++j){
                    model->setData(model->index(i,j),target->data(target->index(i,j)));
                }
        }

    }
}


void MainWindow::on_btnDeleteProject_clicked()
{
    if(ui->combChoiceProject->currentIndex()!=-1){
        int res=QMessageBox::question(0,"警告","你确定要删除当前方案吗?");
        if(res==QMessageBox::StandardButton::Yes){
                m_name2project.remove(ui->combChoiceProject->currentText());
                ui->combChoiceProject->removeItem(ui->combChoiceProject->currentIndex());
                if(ui->combChoiceProject->count()==0){
                    m_optionModel->initTable();
                }
        }
    }
}

