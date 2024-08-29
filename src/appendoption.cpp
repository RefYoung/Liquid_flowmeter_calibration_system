#include "appendoption.h"
#include "ui_appendoption.h"

appendOption::appendOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::appendOption)
{
    ui->setupUi(this);
}

appendOption::~appendOption()
{
    delete ui;
}

void appendOption::on_btnCancel_clicked()
{
    this->reject();
}


void appendOption::on_btnOk_clicked()
{
    emit SignalAppendOption(ui->editName->text(),ui->combMode->currentText(),ui->spinStartSpeed->text(),ui->spinDuration->text(),ui->spinEndSpeed->text(),
                            ui->spinTotal->text(),ui->spinTimes->text(),ui->spinPreInWaterDuration->text(),ui->spinWaitDuration->text(),ui->editRemark->text());
    this->accept();
}


void appendOption::on_combMode_currentIndexChanged(int index)
{
    if(index==0){//总量模式
        ui->spinDuration->setEnabled(false);
        ui->spinEndSpeed->setEnabled(false);
        ui->spinTotal->setEnabled(true);
    }else{//时长模式
        ui->spinDuration->setEnabled(true);
        ui->spinEndSpeed->setEnabled(true);
        ui->spinTotal->setEnabled(false);
    }
}

