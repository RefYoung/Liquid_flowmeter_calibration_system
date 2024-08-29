#include "appendserialinfo.h"
#include "ui_appendserialinfo.h"

AppendSerialInfo::AppendSerialInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppendSerialInfo)
{
    ui->setupUi(this);

    updateSerialPortList();//更新可用串口列表
}

AppendSerialInfo::~AppendSerialInfo()
{
    delete ui;
}

//更新当前可用串口信息
void AppendSerialInfo::updateSerialPortList()
{
    ui->combSerialPort->clear();//清除原有的窗口列表

    QList<QSerialPortInfo>  serialPorts = QSerialPortInfo::availablePorts();//获取当前可用窗口列表信息
    for(QSerialPortInfo& info : serialPorts){//在界面中加入串口信息
        ui->combSerialPort->addItem(info.portName());
    }
}

//当用户点击取消按钮时关闭新建串口窗口
void AppendSerialInfo::on_btnCancel_clicked()
{
    this->reject();
}

//当用户点击确定按钮时发送创建添加串口信息的信号
void AppendSerialInfo::on_btnOk_clicked()
{
    emit appednSerialPortInfo(ui->combSerialPort->currentText(),ui->combBaudrRate->currentText(),ui->combDataBits->currentText(),ui->combParityBit->currentText(),ui->combStopBit->currentText(),ui->combFluidControl->currentText(),ui->editRemark->text());
    this->accept();
}

//当用户点击更新按钮时刷新可用串口列表
void AppendSerialInfo::on_btnUpdateSerialPort_clicked()
{
    updateSerialPortList();
}

