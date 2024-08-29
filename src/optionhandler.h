#ifndef OPTIONHANDLER_H
#define OPTIONHANDLER_H

#include <QStandardItemModel>
#include "appendoption.h"

class OptionHandler : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit OptionHandler(QObject *parent = nullptr);

    //公有方法部分
    void initTable();//初始化表结构,删除所有数据
    void openAppendOptionWindow();//打开测试方案对话框

private:
    void init();//类对象初始函数,请勿在此函数前调用任何自定义方法

private slots:
    void onAppendOption(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
};

#endif // OPTIONHANDLER_H
