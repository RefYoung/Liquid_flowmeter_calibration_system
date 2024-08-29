#include "serialporthandler.h"

SerialPortHandler::SerialPortHandler(QObject* parent) : QStandardItemModel(parent)
{
    init();//调用串口信息表格初始化方法,请不要在此方法前调用任何自定义方法
}

//类初始化函数,类实例化时最先调用的方法
void SerialPortHandler::init()
{
    //初始化表格
    initTable();
}

//串口信息表格初始化方法
//调用此方法后表格数据将被清除,但结构会被保存
void SerialPortHandler::initTable()
{
    //清除原有结构和数据
    this->clear();

    //初始化
    this->setColumnCount(8);
    this->setHeaderData(0,Qt::Horizontal,"名称");
    this->setHeaderData(1,Qt::Horizontal,"波特率");
    this->setHeaderData(2,Qt::Horizontal,"数据位");
    this->setHeaderData(3,Qt::Horizontal,"校验位");
    this->setHeaderData(4,Qt::Horizontal,"停止位");
    this->setHeaderData(5,Qt::Horizontal,"流控");
    this->setHeaderData(6,Qt::Horizontal,"连通性");
    this->setHeaderData(7,Qt::Horizontal,"备注");
}


//打开串口信息设置窗口,并可能添加到表格中
void SerialPortHandler::appendSerialInfo()
{
    AppendSerialInfo* a = new AppendSerialInfo;
    connect(a,SIGNAL(appednSerialPortInfo(QString,QString,QString,QString,QString,QString,QString)),this,SLOT(onAppednSerialPortInfo(QString,QString,QString,QString,QString,QString,QString)));
    a->exec();
    a->deleteLater();
}

//更新连串口的连通性信息
void SerialPortHandler::updateConnectState()
{
    int row=this->rowCount();
    for(int i=0;i<row;++i){

        QSerialPort *_serialPort=connectSerialPort(this->data(this->index(i,0)).toString(),this->data(this->index(i,1)).toString(),
                                                     this->data(this->index(i,2)).toString(),this->data(this->index(i,3)).toString(),
                                                     this->data(this->index(i,4)).toString(),this->data(this->index(i,5)).toString());
        //如果尝试打开成功就设置连通性为可连通
        if(_serialPort!=nullptr)
        {
            this->setData(this->index(i,6),"成功");
            _serialPort->close();
            delete _serialPort;//释放资源
            this->item(i,6)->setBackground(QBrush(Qt::darkGreen));//设置连通性栏背景色
        }else{//否则设置为不可连通
            this->setData(this->index(i,6),"失败");
            this->item(i,6)->setBackground(QBrush(Qt::red));//设置连通性栏背景色
        }
    }
}

//尝试打开串口,打开成功返回串口地址,打开失败返回空指针
//@para   name 串口名
//@para   baute 波特率
//@para   databit 数据位
//@para   paritybit   校验位
//@para   stopbit 停止位
//@para   flowcontrol 流控
QSerialPort *SerialPortHandler::connectSerialPort(QString name, QString baute, QString databit, QString paritybit, QString stopbit, QString flowcontrol)
{
    QSerialPort *serialPort=new QSerialPort(this);

    serialPort->setPortName(name);//设置串口名称
    serialPort->setBaudRate(baute.toUInt());//设置串口波特率
    serialPort->setDataBits((QSerialPort::DataBits)databit.toInt());//设置数据位
    //设置校验位
    if(paritybit == "无校验"){//无校验
        serialPort->setParity(QSerialPort::NoParity);
    }else if(paritybit == "奇校验"){//奇校验
        serialPort->setParity(QSerialPort::OddParity);
    }else if(paritybit == "偶校验"){//偶校验
        serialPort->setParity(QSerialPort::EvenParity);
    }else if(paritybit == "0校验"){//0校验
        serialPort->setParity(QSerialPort::SpaceParity);
    }else if(paritybit == "1校验"){//1校验
        serialPort->setParity(QSerialPort::MarkParity);
    }

    //设置停止位
    if(stopbit == "1"){//1位停止位
        serialPort->setStopBits(QSerialPort::OneStop);
    }else if(stopbit == "1.5"){//1.5位停止位
        serialPort->setStopBits(QSerialPort::OneAndHalfStop);
    }else if(stopbit == "2"){//2位停止位
        serialPort->setStopBits(QSerialPort::TwoStop);
    }

    //设置流控
    if(flowcontrol == "无流控"){//无流控
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }else if(flowcontrol == "软件流控"){//软流控
        serialPort->setFlowControl(QSerialPort::SoftwareControl);
    }else if(flowcontrol == "硬件流控"){//硬流控
        serialPort->setFlowControl(QSerialPort::HardwareControl);
    }

    //如果打开成功返回该串口地址
    if(serialPort->open(QIODevice::ReadWrite)){
        return serialPort;
    }else{//否则返回空指针
        return nullptr;
    }
}


//当用户设置好串口参数并确定后调用此槽函数
//@para   name 串口名
//@para   baute 波特率
//@para   databit 数据位
//@para   paritybit   校验位
//@para   stopbit 停止位
//@para   flowcontrol 流控
//@para   remark  备注
void SerialPortHandler::onAppednSerialPortInfo(QString name, QString baute, QString databit, QString paritybit, QString stopbit, QString flowcontrol, QString remark)
{
    //如果已经添加了该串口则发出提示,并拒绝再添加该串口
    int tempRow=this->rowCount();
    QStringList nameList;
    for(int i=0;i<tempRow;++i){
        nameList << this->data(this->index(i,0)).toString();
    }
    if(nameList.contains(name)){
        QMessageBox::warning(0,"警告","该串口已经添加,请勿重复添加.");
        return;
    }

    //新建行到末尾
    this->insertRow(this->rowCount());

    //插入数据
    int row = this->rowCount() - 1;
    this->setData(this->index(row,0),name);
    this->setData(this->index(row,1),baute);
    this->setData(this->index(row,2),databit);
    this->setData(this->index(row,3),paritybit);
    this->setData(this->index(row,4),stopbit);
    this->setData(this->index(row,5),flowcontrol);
    this->setData(this->index(row,6),"未知");
    this->setData(this->index(row,7),remark);

    this->item(this->rowCount()-1,6)->setBackground(QBrush(Qt::gray));//设置连通性栏背景色


    QSerialPort *_serialPort=connectSerialPort(name,baute,databit,paritybit,stopbit,flowcontrol);
    //如果尝试打开成功就设置连通性为可连通
    if(_serialPort!=nullptr)
    {
        this->setData(this->index(row,6),"成功");
        this->item(row,6)->setBackground(QBrush(Qt::darkGreen));//设置连通性栏背景色
        _serialPort->close();
        delete _serialPort;//释放资源
    }else{//否则设置为不可连通
        this->setData(this->index(row,6),"失败");
        this->item(row,6)->setBackground(QBrush(Qt::red));//设置连通性栏背景色
    }
}
