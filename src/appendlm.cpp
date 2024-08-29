#include "appendlm.h"
#include "ui_appendlm.h"

appendLM::appendLM(QStringList serialPorts,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::appendLM)
{
    ui->setupUi(this);

    //初始化可用串口列表
    ui->combLMSerialPort->addItems(serialPorts);
}

appendLM::~appendLM()
{
    delete ui;
}

void appendLM::on_btnCancel_clicked()
{
    this->reject();
}


void appendLM::on_btnOk_clicked()
{
    emit appendLMSig(ui->editLMName->text(),ui->combLMSerialPort->currentText(),QString::number(ui->spinLMAddress->value()),ui->combLMRole->currentText(),ui->editLMRemark->text());
    this->accept();
}

