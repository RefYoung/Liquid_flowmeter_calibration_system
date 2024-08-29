#ifndef APPENDOPTION_H
#define APPENDOPTION_H

#include <QDialog>

namespace Ui {
class appendOption;
}

class appendOption : public QDialog
{
    Q_OBJECT

public:
    explicit appendOption(QWidget *parent = nullptr);
    ~appendOption();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_combMode_currentIndexChanged(int index);

private:
    Ui::appendOption *ui;

signals:
    void SignalAppendOption(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);

};

#endif // APPENDOPTION_H
