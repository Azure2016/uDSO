#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QSplashScreen>
#include <QDate>
#include <QPainter>
#include <QTimer>
#include <QLayout>
#include <QDir>
#include <QFileDialog>

#include <cmath>

#include "global.h"
#include "window/connectwindow.h"
#include "window/aboutwindow.h"
#include "window/ddswindow.h"
#include "usb/usb.h"
#include "usb/usbthread.h"
#include "component/titlebarwidget.h"
#include "component/statusbarwidget.h"
#include "component/oscillographwidget.h"
#include "component/channelsettingswidget.h"
#include "component/systembutton.h"
#include "component/mainwindowmenu.h"
#include "component/sampleratesettingwidget.h"
#include "component/triggerwidget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // USB通信线程
    USBThread usbThread;

signals:
    void signal_ModeChanged(int);

public slots:
    void showMainMenu();
    void openConnectWindow();
    void openAboutWindow();
    void openDDSWindow();
    void closeApp();
    void updateDisplay(int readIndex);

    void updateStatusBar(QString str);
    void enableButton();
    void disableButton();

    void runButtonHandler();
    void singleButtonHandler();
    void autoButtonHandler();
    void defalutButtonHandler();
    void saveDataDeviceButtonHandler();
    void screenshotButtonHandler();

    // 用于处理控件发送的信号
    // 发送给USB设备
    void voltageOffsetChangedHandler(int chn, int data);
    void voltageRangeChangedHandler(int chn, int data);
    void couplingModeChangedHandler(int chn, int data);
    // 发送给USB设备和OscillographWidget控件
    void triggerChannelChangedHandler(int data);
    void triggerEdgeChangedHandler(int data);
    void triggerLevelChangedHandler(int data);
    // 发送给USB设备
    void sampleRateChangedHandler(int data);

protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::MainWindow *ui;

    // 程序状态控制
    enum{
      RUN     = 0,
      STOP    = 1,
      SINGLE  = 2,
      AUTO    = 3,
      DEFAULT = 4,
    }Mode;

    // 子窗口
    ConnectWindow* connectWindow;
    AboutWindow*   aboutWindow;
    DDSWindow*     ddsWindow;

    QWidget*       central_Widget;

    // 自定义组件
    TitleBarWidget*          title_bar_widget;
    StatusBarWidget*         status_bar_widget;
    MainWindowMenu*          mainMenu;
    OscillographWidget*      oscillograph_widget;
    ChannelSettingsWidget*   ch1_settings_widget;
    ChannelSettingsWidget*   ch2_settings_widget;
    SampleRateSettingWidget* sampleRateSettingWidget;
    TriggerWidget*           triggerWidget;

    // 按键
    SystemButton* runButton;
    SystemButton* singleButton;
    SystemButton* autoButton;
    SystemButton* defalutButton;
    SystemButton* saveDataDeviceButton;
    SystemButton* screenshotButton;

    // 供调试使用
    int demoTimerId;
    int demoDataIndex;
    unsigned char demoDatas[2][4096];

    // 示波器控制参数
    int ch1_VoltageOffset;
    int ch1_VoltageRange;
    int ch1_CouplingMode;
    int ch2_VoltageOffset;
    int ch2_VoltageRange;
    int ch2_CouplingMode;
    int triggerChannel;
    int triggerEdge;
    int triggerLevel;
    long sampleRate;

    void paintEvent(QPaintEvent *);
    void initUI();
    void initLayout();
};

#endif // MAINWINDOW_H
