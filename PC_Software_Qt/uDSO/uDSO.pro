#-------------------------------------------------
#
# Project created by QtCreator 2017-01-20T12:24:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uDSO
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    global.cpp \
    component/channelsettingswidget.cpp \
    component/mainwindowmenu.cpp \
    component/oscillographwidget.cpp \
    component/statusbarwidget.cpp \
    component/systembutton.cpp \
    component/titlebarwidget.cpp \
    window/mainwindow.cpp \
    window/aboutwindow.cpp \
    window/connectwindow.cpp \
    window/ddswindow.cpp \
    usb/usb.cpp \
    usb/usbthread.cpp \
    component/sampleratesettingwidget.cpp \
    component/triggerwidget.cpp

HEADERS  += mainwindow.h \
    global.h \
    component/channelsettingswidget.h \
    component/mainwindowmenu.h \
    component/oscillographwidget.h \
    component/statusbarwidget.h \
    component/systembutton.h \
    component/titlebarwidget.h \
    window/mainwindow.h \
    window/aboutwindow.h \
    window/connectwindow.h \
    window/ddswindow.h \
    usb/usb.h \
    usb/lusb0_usb.h \
    usb/usbthread.h \
    component/sampleratesettingwidget.h \
    component/triggerwidget.h

FORMS    += \
    ui/mainwindow.ui \
    ui/aboutwindow.ui \
    ui/connectwindow.ui \
    ui/ddswindow.ui

RESOURCES += \
    res/img.qrc


LIBS += $${PWD}/lib/libusb.a

RC_FILE = app.rc

DISTFILES +=

CONFIG += resources_big
