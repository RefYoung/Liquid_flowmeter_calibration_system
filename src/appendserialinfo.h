#ifndef APPENDSERIALINFO_H
#define APPENDSERIALINFO_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class AppendSerialInfo;
}

class AppendSerialInfo : public QDialog
{
    Q_OBJECT

public:
    explicit AppendSerialInfo(QWidget *parent = nullptr);
    ~AppendSerialInfo();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_btnUpdateSerialPort_clicked();

private:
    Ui::AppendSerialInfo *ui;

    //私有方法
    void updateSerialPortList();//更新可用串口

signals:
    //当用户确定新建串口信息后发送此信号
    void appednSerialPortInfo(QString name,QString baute,QString databit,QString paritybit,QString stopbit,QString flowcontrol,QString remark);
};

#endif // APPENDSERIALINFO_H
