#include "ddswindow.h"
#include "ui_ddswindow.h"

#include "window/mainwindow.h"

DDSWindow::DDSWindow(QWidget *parent) :
    QWidget(0),
    ui(new Ui::DDSWindow)
{
    parentWidget = parent;

    ui->setupUi(this);

    initUI();
}

DDSWindow::~DDSWindow()
{
    delete ui;
}

void DDSWindow::updateParameter()
{
  DDSControlInfo dds;
  if (parentWidget)
  {
    ((MainWindow *)parentWidget)->usbThread.getDDSParameter(dds);

    if (dds.wave == DDS_WAVE_SINE)
      waveComboBox->setCurrentText("正弦波");
    else if (dds.wave == DDS_WAVE_TRI)
      waveComboBox->setCurrentText("三角波");
    else if (dds.wave == DDS_WAVE_SQUA)
      waveComboBox->setCurrentText("方波");
    else if (dds.wave == DDS_WAVE_SINC)
      waveComboBox->setCurrentText("抽样函数");
    else if (dds.wave == DDS_WAVE_GAUSSIAN)
      waveComboBox->setCurrentText("高斯函数");
    else if (dds.wave == DDS_WAVE_DC)
      waveComboBox->setCurrentText("直流");
    else //if (dds.wave == DDS_WAVE_NOISE)
      waveComboBox->setCurrentText("噪声");

    QString str;
    str.sprintf("%.3lf", dds.frequency);
    freqLineEdit->setText(str);
    str.sprintf("%.3lf", dds.amplitude);
    amplitudeLineEdit->setText(str);
    str.sprintf("%.3lf", dds.offset);
    offsetLineEdit->setText(str);
  }
  else
  {
      freqLineEdit->setText("1000.0");
      amplitudeLineEdit->setText("5.0");
      offsetLineEdit->setText("0.0");
  }

}

void DDSWindow::closeWindow()
{
  this->close();
  this->setAttribute(Qt::WA_DeleteOnClose,true);
}

void DDSWindow::setButtonHandler()
{
    if (parentWidget)
    {
        DDSControlInfo dds;

        if(waveComboBox->currentText() == "正弦波")
          dds.wave = DDS_WAVE_SINE;
        else if(waveComboBox->currentText() == "三角波")
          dds.wave = DDS_WAVE_TRI;
        else if(waveComboBox->currentText() == "方波")
          dds.wave = DDS_WAVE_SQUA;
        else if(waveComboBox->currentText() == "抽样函数")
          dds.wave = DDS_WAVE_SINC;
        else if(waveComboBox->currentText() == "高斯函数")
          dds.wave = DDS_WAVE_GAUSSIAN;
        else if(waveComboBox->currentText() == "直流")
          dds.wave = DDS_WAVE_DC;
        else //if(waveComboBox->currentText() == "噪声")
          dds.wave = DDS_WAVE_NOISE;

        dds.frequency = freqLineEdit->text().toDouble();
        dds.amplitude = amplitudeLineEdit->text().toDouble();
        dds.offset = offsetLineEdit->text().toDouble();
        dds.duty = 50.0;

        ((MainWindow *)parentWidget)->usbThread.setDDSParameter(dds);
    }
}

void DDSWindow::paintEvent(QPaintEvent *)
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

void DDSWindow::initUI()
{
  this->setFixedSize(DEFAULT_DDS_WINDOW_WIDTH, DEFAULT_DDS_WINDOW_HEIGHT);
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
//  this->setWindowIcon(QIcon(":/img/etc/logo.png"));
  this->setWindowIcon(QIcon(":/img/etc/icon.png"));
  this->setWindowTitle("μDSO");
  this->setWindowOpacity(1);  //窗体透明度设置

  titleBarWidget = new TitleBarWidget(this, DDS_WINDOW_TITLE, 30, false);
  titleBarWidget->initConnect();
  connect(titleBarWidget, SIGNAL(closeWindow()), this, SLOT(closeWindow()));

  waveLabel      = new QLabel(this);
  freqLabel      = new QLabel(this);
  amplitudeLabel = new QLabel(this);
  offsetLabel    = new QLabel(this);
  waveLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
  freqLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
  amplitudeLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
  offsetLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
  waveLabel->setMinimumWidth(60);
  freqLabel->setMinimumWidth(60);
  amplitudeLabel->setMinimumWidth(60);
  offsetLabel->setMinimumWidth(60);
  waveLabel->setText("波形");
  freqLabel->setText("频率(Hz)");
  amplitudeLabel->setText("幅度( V )");
  offsetLabel->setText("偏置( V )");

  waveComboBox      = new QComboBox(this);
  waveComboBox->setStyleSheet("QComboBox{"
                               "border: 1px solid gray;"
                               "border-radius: 0px;"
                               " padding: 1px 2px 1px 2px;}"
                               "QComboBox::drop-down{"
                               "subcontrol-origin: padding;"
                               "subcontrol-position: top right;"
                               "width: 20px;"
                               "border-left-width: 1px;"
                               "border-left-color: darkgray;"
                               "border-left-style: solid;"
                               "border-top-right-radius: 5px;"
                               "border-bottom-right-radius: 5px;}"
                               "QComboBox::hover{border-color: rgb(0, 170, 255) ;}"
                               "QComboBox::down-arrow{image: url(:/img/arrow/arrow_down.png);}");
  waveComboBox->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
  waveComboBox->addItem("正弦波");
  waveComboBox->addItem("三角波");
  waveComboBox->addItem("方波");
  waveComboBox->addItem("抽样函数");
  waveComboBox->addItem("高斯函数");
  waveComboBox->addItem("直流");
  waveComboBox->addItem("噪声");

  freqLineEdit      = new QLineEdit(this);
  amplitudeLineEdit = new QLineEdit(this);
  offsetLineEdit    = new QLineEdit(this);
  freqLineEdit->setStyleSheet("QLineEdit { color: black; border: 1px solid  gray;  }");
  amplitudeLineEdit->setStyleSheet("QLineEdit { color: black; border: 1px solid  gray;  }");
  offsetLineEdit->setStyleSheet("QLineEdit { color: black; border: 1px solid  gray;  }");
  freqLineEdit->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
  amplitudeLineEdit->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
  offsetLineEdit->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
  freqLineEdit->setAlignment(Qt::AlignCenter);
  amplitudeLineEdit->setAlignment(Qt::AlignCenter);
  offsetLineEdit->setAlignment(Qt::AlignCenter);
//  freqLineEdit->setValidator(new QDoubleValidator(1.0, 5e6, 6, freqLineEdit));
//  amplitudeLineEdit->setValidator(new QDoubleValidator(0.1, 8.0, 6, amplitudeLineEdit));
//  offsetLineEdit->setValidator(new QDoubleValidator(0.1, 8.0, 6, offsetLineEdit));

  updateParameter();

//  setButton         = new SystemButton(this, "应用设置");
//  enableButton      = new SystemButton(this, "打开输出");
//  setButton->setSize((this->width()- 5*2 - 5)/2, 30);
//  enableButton->setSize((this->width()- 5*2 - 5)/2, 30);
  setButton = new SystemButton(this, "应用设置");
  setButton->setFixedSize(this->width()- 5*2 , 30);

  connect(setButton, SIGNAL(clicked()), this, SLOT(setButtonHandler()));

  initLayout();
}

void DDSWindow::initLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QSpacerItem* vSpacer1   = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QHBoxLayout* hLayout1   = new QHBoxLayout();
    QHBoxLayout* hLayout2   = new QHBoxLayout();
    QHBoxLayout* hLayout3   = new QHBoxLayout();
    QHBoxLayout* hLayout4   = new QHBoxLayout();
    QHBoxLayout* hLayout5   = new QHBoxLayout();

    hLayout1->addWidget(waveLabel, 0);
    hLayout1->addWidget(waveComboBox, 1);
    hLayout1->setSpacing(5);
    hLayout1->setContentsMargins(5, 5, 5, 5);

    hLayout2->addWidget(freqLabel, 0);
    hLayout2->addWidget(freqLineEdit, 1);
    hLayout2->setSpacing(5);
    hLayout2->setContentsMargins(5, 0, 5, 5);

    hLayout3->addWidget(amplitudeLabel, 0);
    hLayout3->addWidget(amplitudeLineEdit, 1);
    hLayout3->setSpacing(5);
    hLayout3->setContentsMargins(5, 0, 5, 5);

    hLayout4->addWidget(offsetLabel,    0);
    hLayout4->addWidget(offsetLineEdit, 1);
    hLayout4->setSpacing(5);
    hLayout4->setContentsMargins(5, 0, 5, 5);

//    hLayout5->addWidget(enableButton, 0);
//    hLayout5->addWidget(setButton   , 1);
    hLayout5->addWidget(setButton, 0);
    hLayout5->setSpacing(5);
    hLayout5->setContentsMargins(5, 0, 5, 5);

    mainLayout->addWidget(titleBarWidget,0);

    mainLayout->addLayout(hLayout1, 1);
    mainLayout->addLayout(hLayout2, 2);
    mainLayout->addLayout(hLayout3, 3);
    mainLayout->addLayout(hLayout4, 4);
    mainLayout->addSpacerItem(vSpacer1);
    mainLayout->addLayout(hLayout5, 5);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(mainLayout);
}
