#include "connectwindow.h"
#include "ui_connectwindow.h"

ConnectWindow::ConnectWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ConnectWindow)
{
  ui->setupUi(this);

  // 初始化界面
  initUI();

  // 禁用本窗口之外的窗口（包括主窗口）
  this->setWindowModality(Qt::ApplicationModal);
}

ConnectWindow::~ConnectWindow()
{
  delete ui;
}

void ConnectWindow::closeWindow()
{
  this->close();
  this->setAttribute(Qt::WA_DeleteOnClose,true);
}

void ConnectWindow::paintEvent(QPaintEvent *)
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

void ConnectWindow::initUI()
{
    QString str;

    this->setFixedSize(DEFAULT_CONNECT_WINDOW_WIDTH, DEFAULT_CONNECT_WINDOW_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
//    this->setWindowIcon(QIcon(":/img/etc/logo.png"));
    this->setWindowIcon(QIcon(":/img/etc/icon.png"));
    this->setWindowTitle("μDSO");
    this->setWindowOpacity(1);  //窗体透明度设置

    titleBarWidget = new TitleBarWidget(this, CONNECT_WINDOW_TITLE, 30, false);
    titleBarWidget->initConnect();
    connect(titleBarWidget, SIGNAL(closeWindow()), this, SLOT(closeWindow()) );

    // 连接状态图标
    imageLabel = new QLabel(this);
    imageLabel->setMaximumSize(128, 128);
    if (g_USBDevice && g_USBDevice->getConnectStatus() == true)
        imageLabel->setStyleSheet("border-image: url(:/img/usb/connected.png);");
    else
        imageLabel->setStyleSheet("border-image: url(:/img/usb/unconnected.png);");

    // 编辑框
    pidLineEdit = new QLineEdit(this);
    vidLineEdit = new QLineEdit(this);
    pidLineEdit->setStyleSheet("QLineEdit { color: black; border: 1px solid  gray;  }");
    vidLineEdit->setStyleSheet("QLineEdit { color: black; border: 1px solid  gray;  }");
    pidLineEdit->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    vidLineEdit->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    pidLineEdit->setAlignment(Qt::AlignCenter);
    vidLineEdit->setAlignment(Qt::AlignCenter);
    pidLineEdit->setText( str.sprintf("0x%04x",g_USBDevice->getDevicePID()) );
    vidLineEdit->setText( str.sprintf("0x%04x",g_USBDevice->getDeviceVID()) );

    // 设备硬件ID
    pidTextLabel = new QLabel(this);
    vidTextLabel = new QLabel(this);
    pidTextLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    vidTextLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    pidTextLabel->setText("设备硬件ID(PID)");
    vidTextLabel->setText("设备硬件ID(VID)");

    // 按键初始化
    connectDeviceButton   = new SystemButton(this, "连接设备");
    unconnectDeviceButton = new SystemButton(this, "断开设备");
    connectDeviceButton->setFixedSize((this->width()- 5*2 - 5)/2, 30);
    unconnectDeviceButton->setFixedSize((this->width()- 5*2 - 5)/2, 30);

    // 设置按键状态
    if ( g_USBDevice && g_USBDevice->getConnectStatus() == false)
    {
        connectDeviceButton->setEnabled(true);
        unconnectDeviceButton->setDisabled(true);
    }
    else
    {
        connectDeviceButton->setDisabled(true);
        unconnectDeviceButton->setEnabled(true);
    }

    //
    connect(connectDeviceButton,   SIGNAL(clicked()), this, SLOT(connectDevice())   );
    connect(unconnectDeviceButton, SIGNAL(clicked()), this, SLOT(unconnectDevice()) );

    initLayout();
}

void ConnectWindow::initLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QSpacerItem* vSpacer1   = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* vSpacer2   = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* hSpacer1   = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* hSpacer2   = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QHBoxLayout* hLayout1   = new QHBoxLayout();
    QHBoxLayout* hLayout2   = new QHBoxLayout();
    QHBoxLayout* hLayout3   = new QHBoxLayout();
    QHBoxLayout* hLayout4   = new QHBoxLayout();

    hLayout1->addSpacerItem(hSpacer1);
    hLayout1->addWidget(imageLabel, 1);
    hLayout1->addSpacerItem(hSpacer2);
    hLayout1->setSpacing(5);
    hLayout1->setContentsMargins(5, 0, 5, 5);

    hLayout2->addWidget(pidTextLabel, 0);
    hLayout2->addWidget(pidLineEdit, 1);
    hLayout2->setSpacing(5);
    hLayout2->setContentsMargins(5, 0, 5, 5);

    hLayout3->addWidget(vidTextLabel, 0);
    hLayout3->addWidget(vidLineEdit, 1);
    hLayout3->setSpacing(5);
    hLayout3->setContentsMargins(5, 0, 5, 5);

    hLayout4->addWidget(connectDeviceButton, 0);
    hLayout4->addWidget(unconnectDeviceButton, 1);
    hLayout4->setSpacing(0);
    hLayout4->setContentsMargins(0, 0, 0, 5);

    mainLayout->addWidget(titleBarWidget,0);
    mainLayout->addSpacerItem(vSpacer1);
    mainLayout->addLayout(hLayout1,1);
    mainLayout->addSpacerItem(vSpacer2);
    mainLayout->addLayout(hLayout2,2);
    mainLayout->addLayout(hLayout3,3);
    mainLayout->addLayout(hLayout4,4);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(mainLayout);
}

void ConnectWindow::connectDevice()
{
    if (ENABLE_SIMULATION_MODE)
        return;

    if (g_USBDevice == NULL)
    {
        qDebug("g_USBDevice 无效指针");
        return;
    }

    if(g_USBDevice->connectDevice() == true)
    {
        emit signal_Connected("已连接到设备！");
        qDebug("已连接到设备！");
        // 设置连接完成标识
        imageLabel->setStyleSheet("border-image: url(:/img/usb/connected.png);");
        // 设置按键
        connectDeviceButton->setDisabled(true);
        unconnectDeviceButton->setEnabled(true);
    }
    else
    {
       qDebug("设备连接失败！");
    }
}

void ConnectWindow::unconnectDevice()
{
    if (ENABLE_SIMULATION_MODE)
        return;

    if (g_USBDevice == NULL)
    {
        qDebug("g_USBDevice 无效指针");
        return;
    }

    if(g_USBDevice->unconnectDevice() == true)
    {
        emit signal_Unconnected("设备断开连接！");
        qDebug("断开设备成功！");
        // 设置断开完成标识
        imageLabel->setStyleSheet("border-image: url(:/img/usb/unconnected.png);");
        // 设置按键状态
        connectDeviceButton->setEnabled(true);
        unconnectDeviceButton->setDisabled(true);
    }
    else
    {
        qDebug("断开设备失败！");
    }
}
