#ifndef APPENDLM_H
#define APPENDLM_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class appendLM;
}

class appendLM : public QDialog
{
    Q_OBJECT
public:
    explicit appendLM(QStringList serialPorts,QWidget *parent = nullptr);
    ~appendLM();

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::appendLM *ui;

signals:
    //信号
    void appendLMSig(QString,QString,QString,QString,QString);//创建下位机信号
};

#endif // APPENDLM_H
