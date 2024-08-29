QT       += core gui serialport xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appendlm.cpp \
    appendoption.cpp \
    appendserialinfo.cpp \
    confighandler.cpp \
    lmhandler.cpp \
    main.cpp \
    mainwindow.cpp \
    optionhandler.cpp \
    serialporthandler.cpp \
    testhandler.cpp

HEADERS += \
    appendlm.h \
    appendoption.h \
    appendserialinfo.h \
    confighandler.h \
    global.h \
    lmhandler.h \
    mainwindow.h \
    optionhandler.h \
    serialporthandler.h \
    testhandler.h

FORMS += \
    appendlm.ui \
    appendoption.ui \
    appendserialinfo.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imgs.qrc
