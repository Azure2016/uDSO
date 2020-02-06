#include "triggerwidget.h"
#include <QDebug>
#include "window/mainwindow.h"

TriggerWidget::TriggerWidget(QWidget *parent):
    QGroupBox(parent)
{
    parentWidget = parent;

    this->setFixedSize(160,125);
    this->setStyleSheet("QGroupBox { "
                       "background: rgb(255, 255, 255); "
                       "border: 1px solid  gray; "
                       "border-color:black;  "
                       "margin-top: 10px; }"

                       "QGroupBox::title { "
                       "color: black; "
                       "subcontrol-origin: margin;  "
                       "subcontrol-position: top left;  "
                       "left:7px; margin-left: 0px;   "
                       "padding: 0 1 px; }");
    this->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    this->setTitle("触发");

    channelLabel = new QLabel(this);
    edgeLabel    = new QLabel(this);
    levelLabel   = new QLabel(this);
    channelLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    edgeLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    levelLabel->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    channelLabel->setText("通道");
    edgeLabel->setText("边沿");
    levelLabel->setText("触发电平");
    channelLabel->setGeometry(10, 25, 80, 25);
    edgeLabel->setGeometry(10,    55, 80, 25);
    levelLabel->setGeometry(10,   85, 120, 25);

    // 下拉菜单样式表
    QString str("QComboBox{"
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
    channelComboBox = new QComboBox(this);
    edgeComboBox    = new QComboBox(this);
    levelComboBox   = new QComboBox(this);
    channelComboBox->setStyleSheet(str);
    edgeComboBox->setStyleSheet(str);
    levelComboBox->setStyleSheet(str);
    channelComboBox->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    edgeComboBox->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    levelComboBox->setFont(QFont("微软雅黑", 10, QFont::Normal, false));

    // 增加按键选项
    channelComboBox->addItem("CH1");
    channelComboBox->addItem("CH2");
    edgeComboBox->addItem("上升沿");
    edgeComboBox->addItem("下降沿");
    for (int i=0; i<=100; i+=10)
    {
        levelComboBox->addItem(str.sprintf("%.2lfV",2.5 * i/100.0 - 1.25));
    }
    // 按键放置
    channelComboBox->setGeometry(50, 25, 100, 25);
    edgeComboBox->setGeometry(   50, 55, 100, 25);
    levelComboBox->setGeometry(  70, 85,  80, 25);

    updateParameter();

    // 设置信号槽
    connect(channelComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(channelComboBoxHandler(const QString & )) );
    connect(edgeComboBox   , SIGNAL(currentTextChanged(const QString &)), this, SLOT(edgeComboBoxHandler(const QString &))    );
    connect(levelComboBox  , SIGNAL(currentTextChanged(const QString &)), this, SLOT(levelComboBoxHandler(const QString &))   );
}

TriggerWidget::~TriggerWidget()
{

}

void TriggerWidget::updateParameter()
{
  QString str;

  DSOControlInfo dso;
  ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
  int channel=(int)dso.trigChannel;
  int edge=(int)dso.trigEdge;
  double level=dso.trigLevel;

  if (channel == DSO_CHANNEL1)
    channelComboBox->setCurrentText("CH1");
  else
    channelComboBox->setCurrentText("CH2");

  if (edge == DSO_TRIG_RAISE)
    channelComboBox->setCurrentText("上升沿");
  else
    channelComboBox->setCurrentText("下降沿");

  str.sprintf("%.2lfV",level-1.25);
  levelComboBox->setCurrentText(str);
}

void TriggerWidget::channelComboBoxHandler(const QString & data)
{
  DSOControlInfo dso;
  ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);

  if(data == "CH1")
      dso.trigChannel = DSO_CHANNEL1;
  else // if(data == "CH2")
      dso.trigChannel = DSO_CHANNEL2;

  ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

//    emit signal_channelChanged(res);
}

void TriggerWidget::edgeComboBoxHandler(const QString & data)
{
  DSOControlInfo dso;
  ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);

  if(data == "上升沿")
      dso.trigEdge = DSO_TRIG_RAISE;
  else //if (data == "下降沿")
      dso.trigEdge = DSO_TRIG_FALL;

  ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

//    emit signal_edgeChanged(res);
}

void TriggerWidget::levelComboBoxHandler(const QString & data)
{
  DSOControlInfo dso;
  ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);

  QString str;
  for (int i = 0; i < 100; i+=10)
  {
      str.sprintf("%.2lfV", 2.5 * i / 100.0 - 1.25);
      if (data == str)
      {
          dso.trigLevel = 2.5 * i / 100.0;
          break;
      }
  }

  ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

//    emit signal_levelChanged(res);
}
