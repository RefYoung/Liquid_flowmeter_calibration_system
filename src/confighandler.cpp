#include "confighandler.h"

ConfigHandler::ConfigHandler(QObject* parent):
    QObject(parent)
{
    serialPortHandler=new SerialPortHandler(this);
    lmHandler=new LMHandler(this);
    optionHandler= new OptionHandler(this);
}
