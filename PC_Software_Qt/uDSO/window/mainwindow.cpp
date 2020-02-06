#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化窗口指针
    aboutWindow   = NULL;
    connectWindow = NULL;
    ddsWindow     = NULL;

    // 主界面模式设置为运行模式
    Mode = RUN;

    // 实例化USB类
    g_USBDevice = new usb();
    // 主界面接收线程发送的信号
    connect(&usbThread,
            SIGNAL(signal_ReviceFinish(int)),
            this,
            SLOT(updateDisplay(int)) );

    // 初始化UI
    initUI();

    // 参数初始化
    ch1_VoltageOffset = 0;
    ch1_VoltageRange  = 0;
    ch1_CouplingMode  = 0;
    ch2_VoltageOffset = 0;
    ch2_VoltageRange  = 0;
    ch2_CouplingMode  = 0;
    triggerChannel    = 0;
    triggerEdge       = 0;
    triggerLevel      = 0;
    sampleRate        = 100000000;

    // 通道设置信号处理
    connect(ch1_settings_widget, SIGNAL(voltageOffsetChanged(int, int)), this, SLOT(voltageOffsetChangedHandler(int, int)) );
    connect(ch1_settings_widget, SIGNAL(voltageRangeChanged(int, int)) , this, SLOT(voltageRangeChangedHandler(int, int))  );
    connect(ch1_settings_widget, SIGNAL(couplingModeChanged(int, int)) , this, SLOT(couplingModeChangedHandler(int, int))  );
    connect(ch2_settings_widget, SIGNAL(voltageOffsetChanged(int, int)), this, SLOT(voltageOffsetChangedHandler(int, int)) );
    connect(ch2_settings_widget, SIGNAL(voltageRangeChanged(int, int)) , this, SLOT(voltageRangeChangedHandler(int, int))  );
    connect(ch2_settings_widget, SIGNAL(couplingModeChanged(int, int)) , this, SLOT(couplingModeChangedHandler(int, int))  );
    // 采样率设置信号处理
    connect(sampleRateSettingWidget, SIGNAL(sampleRateChanged(int)), this, SLOT(sampleRateChangedHandler(int)) );
    // 触发设置信号处理
    connect(triggerWidget, SIGNAL(signal_channelChanged(int)), this, SLOT(triggerChannelChangedHandler(int)) );
    connect(triggerWidget, SIGNAL(signal_edgeChanged(int))   , this, SLOT(triggerEdgeChangedHandler(int))    );
    connect(triggerWidget, SIGNAL(signal_levelChanged(int))  , this, SLOT(triggerLevelChangedHandler(int))   );

    // 启动线程
    usbThread.start();
}

MainWindow::~MainWindow()
{
    if (g_USBDevice)
    {
        // 关闭USB
        g_USBDevice->unconnectDevice();
        delete g_USBDevice;
        g_USBDevice = NULL;
    }

    if (aboutWindow)
    {
        delete aboutWindow;
        aboutWindow = NULL;
    }
    if (connectWindow)
    {
        delete connectWindow;
        connectWindow = NULL;
    }
    if (ddsWindow)
    {
        delete ddsWindow;
        ddsWindow = NULL;
    }

    delete ui;

    usbThread.stop();

}

// 窗口重定义大小
void MainWindow::resizeEvent(QResizeEvent *)
{
}

// 界面绘制事件
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(0, 0, this->width(), this->height());

    QPainter painter(this);
    painter.fillPath(path, QBrush(Qt::white));

    path.setFillRule(Qt::WindingFill);
    path.addRect(0, 0, this->width()-1, this->height()-1);

    painter.setPen("#0063B1");
    painter.drawPath(path);
}

// 初始化界面
void MainWindow::initUI()
{
    QString titleText = MAIN_WINDOW_TITLE;
    if (ENABLE_SIMULATION_MODE)
        titleText += tr(" - Simulation Mode");

    this->setMinimumSize(DEFAULT_MAIN_WINDOW_WIDTH, DEFAULT_MAIN_WINDOW_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
//    this->setWindowIcon(QIcon(":/img/etc/logo.png"));
    this->setWindowIcon(QIcon(":/img/etc/icon.png"));
    this->setWindowTitle(titleText);
    this->setWindowOpacity(1);  //窗体透明度设置

    // 标题栏
    title_bar_widget = new TitleBarWidget(this, titleText, DEFAULT_TITLE_BAR_HEIGHT, true);
    title_bar_widget->initConnect();
    connect(title_bar_widget, SIGNAL(closeWindow()), this, SLOT(closeApp()));

    // 主界面的下拉菜单
    mainMenu = new MainWindowMenu(this);
    mainMenu->intConnect();

    // 状态栏
    status_bar_widget = new StatusBarWidget(this, DEFAULT_STATUS_BAR_HEIGHT);
    status_bar_widget->showMessage("未连接设备！");
    status_bar_widget->isEnabledShowTime(true);

    // 波形显示区域
    oscillograph_widget = new OscillographWidget(this, 2);
    oscillograph_widget->setMinimumSize(this->width()-160-5*3,
                                        this->height()-title_bar_widget->height()-status_bar_widget->height() - 5*2);
    //连接信号槽函数
    connect(this, SIGNAL(signal_ModeChanged(int)), oscillograph_widget, SLOT(updateMode(int)));
    emit signal_ModeChanged(Mode);

    // CH1、CH2控制面板
    ch1_settings_widget = new ChannelSettingsWidget(this, 0, "CH1", Qt::yellow);
    ch2_settings_widget = new ChannelSettingsWidget(this, 1, "CH2", QColor(0x48, 0x76, 0xFF));

    // 设置颜色关联的槽函数
    connect(ch1_settings_widget,
            SIGNAL(markColoChange(int, QColor)),
            oscillograph_widget,
            SLOT(setChannelMarkColor(int, QColor))
           );
    connect(ch2_settings_widget,
            SIGNAL(markColoChange(int, QColor)),
            oscillograph_widget,
            SLOT(setChannelMarkColor(int, QColor))
           );
    // 直接调用槽函数设置波形颜色
    oscillograph_widget->setChannelMarkColor(0, Qt::yellow);
    oscillograph_widget->setChannelMarkColor(1, QColor(0x48, 0x76, 0xFF));

    // 采样速率设置
    sampleRateSettingWidget = new SampleRateSettingWidget(this);

    // 触发设置
    triggerWidget = new TriggerWidget(this);

    // 按键
    runButton            = new SystemButton(this, "运行/暂停");
//    singleButton         = new SystemButton(this, "单次采集");
//    autoButton           = new SystemButton(this, "自动设置");
    defalutButton        = new SystemButton(this, "默认设置");
//    saveDataDeviceButton = new SystemButton(this, "保存数据");
    screenshotButton     = new SystemButton(this, "截图");

    // 设置按键默认状态
    disableButton();

    // 设置按键事件
    connect(runButton,            SIGNAL(clicked()), this, SLOT( runButtonHandler())            );
//    connect(singleButton,         SIGNAL(clicked()), this, SLOT( singleButtonHandler())         );
//    connect(autoButton,           SIGNAL(clicked()), this, SLOT( autoButtonHandler())           );
    connect(defalutButton,        SIGNAL(clicked()), this, SLOT( defalutButtonHandler())        );
//    connect(saveDataDeviceButton, SIGNAL(clicked()), this, SLOT( saveDataDeviceButtonHandler()) );
    connect(screenshotButton,     SIGNAL(clicked()), this, SLOT( screenshotButtonHandler())     );

    connect(ch1_settings_widget, SIGNAL(switchStateChanged(int, bool)), oscillograph_widget, SLOT(setChannelDisplay(int, bool)));
    connect(ch2_settings_widget, SIGNAL(switchStateChanged(int, bool)), oscillograph_widget, SLOT(setChannelDisplay(int, bool)));

    // 初始化布局
    initLayout();
}

// 初始化布局
void MainWindow::initLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *hLayout1   = new QHBoxLayout();
    QVBoxLayout *vLayout1   = new QVBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();
    QSpacerItem *vSpacer1   = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
//    QSpacerItem *vSpacer2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

//    gridLayout->addWidget(runButton, 0, 0);
//    gridLayout->addWidget(singleButton, 0, 1);
//    gridLayout->addWidget(autoButton, 1, 0);
//    gridLayout->addWidget(defalutButton, 1, 1);
//    gridLayout->addWidget(saveDataDeviceButton, 2, 0);
//    gridLayout->addWidget(screenshotButton, 2, 1);
//    gridLayout->setSpacing(5);
//    gridLayout->setContentsMargins(0, 0, 0, 0);
    runButton->setFixedSize(160, 32);
    defalutButton->setFixedSize(160, 32);
    screenshotButton->setFixedSize(160, 32);
    gridLayout->addWidget(runButton, 0, 0);
    gridLayout->addWidget(defalutButton, 1, 0);
    gridLayout->addWidget(screenshotButton, 2, 0);
    gridLayout->setSpacing(2);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    vLayout1->addWidget(ch1_settings_widget, 0);
    vLayout1->addWidget(ch2_settings_widget, 1);
    vLayout1->addWidget(sampleRateSettingWidget, 2);
    vLayout1->addWidget(triggerWidget, 3);
    vLayout1->addSpacerItem(vSpacer1);
    vLayout1->addLayout(gridLayout);
//    vLayout1->addSpacerItem(vSpacer2);
    vLayout1->setSpacing(0);
    vLayout1->setContentsMargins(5, 0, 5, 5);

    hLayout1->addWidget(oscillograph_widget,0);
    hLayout1->addLayout(vLayout1,1);
    hLayout1->setSpacing(0);
    hLayout1->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(title_bar_widget,0);
    mainLayout->addLayout(hLayout1,1);
    mainLayout->addWidget(status_bar_widget,2);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(1, 1, 1, 1);

    central_Widget = new QWidget();
    this->setCentralWidget(central_Widget);
    central_Widget->setLayout(mainLayout);
}

// 打开下拉菜单
void MainWindow::showMainMenu()
{
    QPoint p = this->rect().topRight();

    p.setX(p.x() - 180);
    p.setY(p.y() + title_bar_widget->height());
    mainMenu->exec(this->mapToGlobal(p));
}

// 打开设备连接窗口
void MainWindow::openConnectWindow()
{
    if (connectWindow == NULL)
    {
        connectWindow = new ConnectWindow();
        connectWindow->show();
    }
    else if (connectWindow->testAttribute(Qt::WA_DeleteOnClose) == true)
    {
        delete connectWindow;
        connectWindow = new ConnectWindow();
        connectWindow->show();
    }
    connect( connectWindow, SIGNAL( signal_Connected(QString)   ),   this, SLOT( updateStatusBar(QString) ) );
    connect( connectWindow, SIGNAL( signal_Unconnected(QString) ),   this, SLOT( updateStatusBar(QString) ) );
    connect( connectWindow, SIGNAL( signal_Connected(QString)   ),   this, SLOT( enableButton(QString)    ) );
    connect( connectWindow, SIGNAL( signal_Unconnected(QString) ),   this, SLOT( disableButton(QString)   ) );
}

// 打开帮助窗口
void MainWindow::openAboutWindow()
{
    if (aboutWindow == NULL)
    {
        aboutWindow = new AboutWindow(0);
        aboutWindow->show();
    }
    else if (aboutWindow->testAttribute(Qt::WA_DeleteOnClose) == true)
    {
        delete aboutWindow;
        aboutWindow = new AboutWindow(0);
        aboutWindow->show();
    }
}

// 打开信号发生器窗口
void MainWindow::openDDSWindow()
{
    if (ddsWindow == NULL)
    {
        ddsWindow = new DDSWindow(this);
        ddsWindow->show();
    }
    else if (ddsWindow->testAttribute(Qt::WA_DeleteOnClose) == true)
    {
        delete ddsWindow;
        ddsWindow = new DDSWindow(this);
        ddsWindow->show();
    }
}

// 关闭应用
void MainWindow::closeApp()
{
    // 关闭其它窗口
    if (aboutWindow && aboutWindow->testAttribute(Qt::WA_DeleteOnClose) == false)
    {
        aboutWindow->close();
    }
    if (connectWindow && connectWindow->testAttribute(Qt::WA_DeleteOnClose) == false)
    {
        connectWindow->close();
    }
    if (ddsWindow && ddsWindow->testAttribute(Qt::WA_DeleteOnClose) == false)
    {
        ddsWindow->close();
    }

    // 切断设备连接

    // 关闭主窗口
    this->close();
}

// 更新波形界面
void MainWindow::updateDisplay(int readIndex)
{
    if ( Mode == RUN || Mode == SINGLE || Mode == AUTO || Mode == DEFAULT )
    {
        usbThread.setReadIndex(readIndex);
        oscillograph_widget->dispWaveData((unsigned char*)(usbThread.adData[readIndex]), SAMPLING_LENGTH_MAX);
        usbThread.setReadIndex(-1);
    }
}

// 更新消息栏
void MainWindow::updateStatusBar(QString str)
{
    status_bar_widget->showMessage(str);
}

// 失效某些按键
void MainWindow::enableButton()
{
    runButton->setEnabled(true)            ;
//    singleButton->setEnabled(true)         ;
//    autoButton->setEnabled(true)           ;
    defalutButton->setEnabled(true)        ;
//    saveDataDeviceButton->setEnabled(true) ;
}

// 所有按键有效
void MainWindow::disableButton()
{
    runButton->setDisabled(true)            ;
//    singleButton->setDisabled(true)         ;
//    autoButton->setDisabled(true)           ;
    defalutButton->setDisabled(true)        ;
//    saveDataDeviceButton->setDisabled(true) ;
}

// 运行状态设置
void MainWindow::runButtonHandler()
{
    if (Mode == RUN)
        Mode = STOP;
    else
        Mode = RUN;

    emit signal_ModeChanged(Mode);
}

// 单次模式触发
void MainWindow::singleButtonHandler()
{
    Mode = SINGLE;
    emit signal_ModeChanged(Mode);
}

// 自动设置
void MainWindow::autoButtonHandler()
{
    Mode = AUTO;
    emit signal_ModeChanged(Mode);
}

// 默认设置
void MainWindow::defalutButtonHandler()
{
//    Mode = DEFAULT;
//    emit signal_ModeChanged(Mode);

  // DDS参数
  DDSControlInfo dds;
  dds.wave      = DDS_WAVE_SINE;
  dds.frequency = 1000.0;
  dds.amplitude = 5.0;
  dds.offset    = 0.0;
  dds.duty      = 50.0;
  usbThread.setDDSParameter(dds);
  // DSO参数
  DSOControlInfo dso;
  dso.sampleRate = 8;
  dso.ch1Offset = 0.0;
  dso.ch1VoltageRange = 2;
  dso.ch1Coupling = DSO_COUPLING_DC;
  dso.ch2Offset =  0.0;
  dso.ch2VoltageRange = 2;
  dso.ch2Coupling = DSO_COUPLING_DC;
  dso.trigChannel = DSO_CHANNEL1;
  dso.trigEdge = DSO_TRIG_RAISE;
  dso.trigLevel = 1.25;
  usbThread.setDSOParameter(dso);

  if (ddsWindow && ddsWindow->testAttribute(Qt::WA_DeleteOnClose) == false)
    ddsWindow->updateParameter();

  ch1_settings_widget->updateParameter();
  ch2_settings_widget->updateParameter();
  sampleRateSettingWidget->updateParameter();
  triggerWidget->updateParameter();
}

// 保存数据
void MainWindow::saveDataDeviceButtonHandler()
{

}

// 截图按键处理
void MainWindow::screenshotButtonHandler()
{
    QPixmap  currentScreenPixmap(this->width(),this->height()+20);
    QPainter painter(&currentScreenPixmap);

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save screenshot"),
                                                    QDateTime::currentDateTime().toString("yyyyMMddhhmmss"),
                                                    tr("Image Files (*.png)"));

    if (!fileName.isNull())
    {
        painter.fillRect(QRect(0,0,currentScreenPixmap.width(),currentScreenPixmap.height()),Qt::white);
        painter.drawPixmap(0, 20, QWidget::grab(QRect(0,0,this->width(),this->height()) ) );
        painter.setFont(QFont("微软雅黑",10,QFont::Normal,false));

        painter.setPen(Qt::black);
        painter.drawText(QRect(0,0,800,20),(QString)" μDSO   " +
                         (QString)"Version:" +
                         (QString)UDSO_VERSION +
                         (QString)"   " +
                         QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
        painter.drawText(QRect(currentScreenPixmap.width()-30,0,30,20),"截图");

        currentScreenPixmap.save(fileName);
        qDebug()<< fileName;

        status_bar_widget->showMessage("已保存到: " + fileName);
    }
}

// 电压偏移设置
void MainWindow::voltageOffsetChangedHandler(int chn, int data)
{
    switch(chn)
    {
        case 0:
            // TODO 修改偏移，检查参数是否在合适的范围内
            // TODO 修改面板显示的内容
            // TODO 发送CH1偏移修改信息给USB设备
            break;
        case 1:
            // TODO 修改偏移，检查参数是否在合适的范围内
            // TODO 修改面板显示的内容
            // TODO 发送CH2偏移修改信息给USB设备
            break;
        defalut:
            break;
    }
}

// 电压量程设置
void MainWindow::voltageRangeChangedHandler(int chn, int data)
{
    switch(chn)
    {
    case 0:
        // TODO 修改量程，检查参数是否在合适的范围内
        // TODO 修改面板显示的内容
        // TODO 发送CH1量程修改信息给USB设备
        break;
    case 1:
        // TODO 修改量程，检查参数是否在合适的范围内
        // TODO 修改面板显示的内容
        // TODO 发送CH2量程修改信息给USB设备
        break;
defalut:
        break;
    }
}

// 耦合模式设置
void MainWindow::couplingModeChangedHandler(int chn, int data)
{
    switch(chn)
    {
        case 0:
            // TODO 修改耦合，检查参数是否在合适的范围内
            // TODO 修改面板显示的内容
            // TODO 发送CH1耦合修改信息给USB设备
            break;
        case 1:
            // TODO 修改耦合，检查参数是否在合适的范围内
            // TODO 修改面板显示的内容
            // TODO 发送CH2耦合修改信息给USB设备
            break;
        defalut:
            break;
    }
}

// 触发通道设置
void MainWindow::triggerChannelChangedHandler(int data)
{

}

// 触发边沿
void MainWindow::triggerEdgeChangedHandler(int data)
{

}

// 触发水平设置
void MainWindow::triggerLevelChangedHandler(int data)
{

}

// 采样率改变
void MainWindow::sampleRateChangedHandler(int data)
{
    static int count=16;

    if (data == 1 && count < 16)
    {
        count+=1;
    }
    else if (data == -1 && count >0)
    {
        count-=1;
    }

    QString sampleRateString;
    QString resolutionString;
    switch (count)
    {
        case 0:
            sampleRateString = "500/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(5*10e2)*10e3);
            break;
        case 1:
            sampleRateString = "1K/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(1*10e3)*10e3);
            break;
        case 2:
            sampleRateString = "2.5K/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(2.5*10e3)*10e3);
            break;
        case 3:
            sampleRateString = "5K/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(5*10e3)*10e3);
            break;
        case 4:
            sampleRateString = "10K/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(1*10e4)*10e3);
            break;
        case 5:
            sampleRateString = "25K/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(2.5*10e4)*10e3);
            break;
        case 6:
            sampleRateString = "50K/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(5*10e4)*10e3);
            break;
        case 7:
            sampleRateString = "100K/s";
            resolutionString.sprintf("%.0fms/Div",204.8/(1*10e5)*10e3);
            break;
        case 8:
            sampleRateString = "250K/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(2.5*10e5)*10e6);
            break;
        case 9:
            sampleRateString = "500K/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(5*10e5)*10e6);
            break;
        case 10:
            sampleRateString = "1M/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(1*10e6)*10e6);
            break;
        case 11:
            sampleRateString = "2.5M/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(2.5*10e6)*10e6);
            break;
        case 12:
            sampleRateString = "5M/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(5*10e6)*10e6);
            break;
        case 13:
            sampleRateString = "10M/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(1*10e7)*10e6);
            break;
        case 14:
            sampleRateString = "25M/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(2.5*10e7)*10e6);
            break;
        case 15:
            sampleRateString = "50M/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(5*10e7)*10e6);
            break;
        case 16:
            sampleRateString = "100M/s";
            resolutionString.sprintf("%.0fus/Div",204.8/(1*10e8)*10e6);
            break;
        default:
            break;
    }

    sampleRateSettingWidget->setSampleRateDisplay(sampleRateString);
    sampleRateSettingWidget->setResolutionDisplay(resolutionString);
}
