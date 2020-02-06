#include "channelsettingswidget.h"
#include <QDebug>
#include <QString>
#include "window/mainwindow.h"

ChannelSettingsWidget::ChannelSettingsWidget(QWidget *parent, int mark, QString str, QColor color):
  QGroupBox(parent)
{
  parentWidget = parent;
  channelMark   = mark;

  this->setFixedSize(160,160);
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
  this->setTitle(str);

  //
  knob1 = new QWidget(this);
  knob1Count = 0;
  knob1->setFixedSize(64, 64);
  knob1->setStyleSheet("border-image: url(:/img/knob/knob0.png);");
  knob1->move(95,20);

  knob2 = new QWidget(this);
  knob2Count = 0;
  knob2->setFixedSize(64, 64);
  knob2->setStyleSheet("border-image: url(:/img/knob/knob0.png);");
  knob2->move(95,90);

  // 标签
  voltageRangeLabel = new QLabel(this);
  voltageRangeLabel->setFrameShape(QFrame::Box);
  voltageRangeLabel->setStyleSheet("QLabel { color: blue; border: 1px solid  gray;  }");
  voltageRangeLabel->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
  voltageRangeLabel->setText("2v/Div");
  voltageRangeLabel->setGeometry(10, knob1->y()+5, 80, 25);
  voltageRangeLabel->setAlignment(Qt::AlignCenter);

  // 标签
  VoltageOffsetLabel = new QLabel(this);
  VoltageOffsetLabel->setFrameShape(QFrame::Box);
  VoltageOffsetLabel->setStyleSheet("QLabel { color: blue;  border: 1px solid  gray;}");
  VoltageOffsetLabel->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
  VoltageOffsetLabel->setText("+0mv");
  VoltageOffsetLabel->setGeometry(10, knob2->y()+5, 80, 25);
  VoltageOffsetLabel->setAlignment(Qt::AlignCenter);

  // 按键
  switchButton = new SystemButton(this, "ON");
  switchButton->setFont(QFont("微软雅黑", 9, QFont::Bold, false));
  switchButton->move(10, knob1->y()+35);
  switchButton->setFixedSize(35, switchButton->height());

  // 耦合方式
  couplingButton = new SystemButton(this, "AC");
  couplingButton->setFont(QFont("微软雅黑", 8, QFont::Bold, false));
  couplingButton->move(50, knob1->y()+35);
  couplingButton->setFixedSize(40, couplingButton->height());

  // 设置颜色标签
  markColorLabel = new QLabel(this);
  markColorLabel->setFrameShape(QFrame::Box);
  setMarkColor(color);
  markColorLabel->setGeometry(10,knob2->y()+35, 25, 25);

  // 颜色标记按键
  markColorButton = new SystemButton(this, "COLOR");
  markColorButton->setFont(QFont("微软雅黑", 8, QFont::Bold, false));
  markColorButton->move(40, knob2->y()+35);
  markColorButton->setFixedSize(50, markColorButton->height());

  // 显示DSO信息
  updateParameter();

  // 关联按键的信号和槽
  connect(switchButton,    SIGNAL(clicked()), this, SLOT(switchButtonHandler())    );
  connect(couplingButton,  SIGNAL(clicked()), this, SLOT(couplingButtonHandler())  );
  connect(markColorButton, SIGNAL(clicked()), this, SLOT(markColorButtonHandler()) );
}

ChannelSettingsWidget::~ChannelSettingsWidget()
{

}

void ChannelSettingsWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->x() >= knob1->x()
      && event->x() <= knob1->x() + knob1->width()
      && event->y() >= knob1->y()
      && event->y() <= knob1->y() + knob1->height()
      )
    {
      DSOControlInfo dso;
      ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
      int range;

      if (channelMark == 0)
          range = (int)dso.ch1VoltageRange;
      else // if(channelMark == 1)
          range = (int)dso.ch2VoltageRange;

      if (event->button() == Qt::LeftButton)
      {
        knob1Count = (knob1Count+1)%8;
        range = (range >= (DSO_VOLTAGE_RANGE_NUM-1)) ? (DSO_VOLTAGE_RANGE_NUM-1) : (range+1);
        emit voltageRangeChanged(channelMark, 1);
      }
      else if (event->button() == Qt::RightButton)
      {
        knob1Count = knob1Count <= 0 ? 7 : knob1Count-1;
        range = (range <= 0) ? 0 : (range-1);
        emit voltageRangeChanged(channelMark, -1);
      }

      voltageRangeLabel->setText(textVoltageRange[range][1]);
      if (channelMark == 0)
          dso.ch1VoltageRange = range;
      else // if(channelMark == 1)
          dso.ch2VoltageRange = range;
      ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

      // 更新滚轮状态
      updateKnob(knob1);
    }
    else if(event->x() >= knob2->x()
          && event->x() <= knob2->x() + knob2->width()
          && event->y() >= knob2->y()
          && event->y() <= knob2->y() + knob2->height()
          )
    {
        DSOControlInfo dso;
        ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
        double offset;

        if (channelMark == 0)
            offset = dso.ch1Offset;
        else // if(channelMark == 1)
            offset = dso.ch2Offset;

      if (event->button() == Qt::LeftButton)
      {
        knob2Count = (knob2Count+1)%8;
        offset = (offset >= 2.5) ? 2.5 : (offset+0.1);
        emit voltageOffsetChanged(channelMark, 1);
      }
      else if (event->button() == Qt::RightButton)
      {
        knob2Count = knob2Count <= 0 ? 7 : knob2Count-1;
        offset = (offset <= -2.5) ? -2.5 : (offset-0.1);
        emit voltageOffsetChanged(channelMark, -1);
      }

      QString str;
      if (channelMark == 0)
        str.sprintf("%.1lfV", offset / 2.5 * voltageRangeValue[(int)dso.ch1VoltageRange]);
      else // if(channelMark == 1)
        str.sprintf("%.1lfV", offset / 2.5 * voltageRangeValue[(int)dso.ch2VoltageRange]);
      VoltageOffsetLabel->setText(str);

      if (channelMark == 0)
          dso.ch1Offset = offset;
      else // if(channelMark == 1)
          dso.ch2Offset = offset;
      ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

      // 更新滚轮状态
      updateKnob(knob2);

    }
}

void ChannelSettingsWidget::wheelEvent(QWheelEvent* event)
{  
  if (event->x() >= knob1->x()
      && event->x() <= knob1->x() + knob1->width()
      && event->y() >= knob1->y()
      && event->y() <= knob1->y() + knob1->height()
      )
  {
      DSOControlInfo dso;
      ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
      int range;

      if (channelMark == 0)
          range = (int)dso.ch1VoltageRange;
      else // if(channelMark == 1)
          range = (int)dso.ch2VoltageRange;

      if (event->delta() > 0)
      {
        knob1Count = (knob1Count+1)%8;
        range = (range >= (DSO_VOLTAGE_RANGE_NUM-1)) ? (DSO_VOLTAGE_RANGE_NUM-1) : (range+1);
        emit voltageRangeChanged(channelMark, 1);
      }
      else
      {
        knob1Count = knob1Count <= 0 ? 7 : knob1Count-1;
        range = (range <= 0) ? 0 : (range-1);
        emit voltageRangeChanged(channelMark, -1);
      }

      voltageRangeLabel->setText(textVoltageRange[range][1]);
      if (channelMark == 0)
          dso.ch1VoltageRange = range;
      else // if(channelMark == 1)
          dso.ch2VoltageRange = range;
      ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

      // 更新滚轮状态
      updateKnob(knob1);

    }
  else if(event->x() >= knob2->x()
          && event->x() <= knob2->x() + knob2->width()
          && event->y() >= knob2->y()
          && event->y() <= knob2->y() + knob2->height()
          )
    {
      DSOControlInfo dso;
      ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
      double offset;

      if (channelMark == 0)
          offset = dso.ch1Offset;
      else // if(channelMark == 1)
          offset = dso.ch2Offset;

      if (event->delta() > 0)
      {
        knob2Count = (knob2Count+1)%8;
        offset = (offset >= 2.5) ? 2.5 : (offset+0.1);
        emit voltageOffsetChanged(channelMark, 1);
      }
      else
      {
        knob2Count = knob2Count <= 0 ? 7 : knob2Count-1;
        offset = (offset <= -2.5) ? -2.5 : (offset-0.1);
        emit voltageOffsetChanged(channelMark, -1);
      }

      QString str;
      if (channelMark == 0)
        str.sprintf("%.1lfV", offset / 2.5 * voltageRangeValue[(int)dso.ch1VoltageRange]);
      else // if(channelMark == 1)
        str.sprintf("%.1lfV", offset / 2.5 * voltageRangeValue[(int)dso.ch2VoltageRange]);
      VoltageOffsetLabel->setText(str);

      if (channelMark == 0)
          dso.ch1Offset = offset;
      else // if(channelMark == 1)
          dso.ch2Offset = offset;
      ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

      // 更新滚轮状态
      updateKnob(knob2);
    }

}

void ChannelSettingsWidget::updateKnob(QWidget *widget)
{
  QString str;

  if (widget == knob1)
    str.sprintf("border-image: url(:/img/knob/knob%d.png);",knob1Count);
  else if (widget == knob2)
    str.sprintf("border-image: url(:/img/knob/knob%d.png);",knob2Count);

  widget->setStyleSheet(str);
}

void ChannelSettingsWidget::setMarkColor(QColor color)
{
    markColor = color;

    // 设置标签颜色
    markColorLabel->setStyleSheet( QString("QLabel { background-color: rgb(%1, %2, %3);  border: 1px solid  gray;}")\
                                   .arg(markColor.red()).arg(markColor.green()).arg(markColor.blue()) );

    // 发射信号通知颜色改变
    emit markColoChange(channelMark, markColor);
}

// 设置面板显示的数据（电压偏移）
void ChannelSettingsWidget::setVoltageOffsetDisplay(QString str)
{
    voltageRangeLabel->setText(str);
}

// 设置面板显示的数据（电压量程）
void ChannelSettingsWidget::setVoltageRangeDisplay(QString str)
{
    voltageRangeLabel->setText(str);
}

QColor ChannelSettingsWidget::getMarkColor()
{
    return markColor;
}

void ChannelSettingsWidget::updateParameter()
{
  QString str;
  DSOControlInfo dso;
  ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
  int coupling, range;
  double offset;

  if (channelMark == 0)
  {
      coupling = (int)dso.ch1Coupling;
      range = (int)dso.ch1VoltageRange;
      offset = dso.ch1Offset;
  }
  else // if(channelMark == 1)
  {
      coupling = (int)dso.ch2Coupling;
      range = (int)dso.ch2VoltageRange;
      offset = dso.ch2Offset;
  }

  if (coupling == DSO_COUPLING_DC)
    couplingButton->setText("DC");
  else
    couplingButton->setText("AC");

  voltageRangeLabel->setText(textVoltageRange[range][1]);

  if (channelMark == 0)
    str.sprintf("%.1lfV", offset / 2.5 * voltageRangeValue[(int)dso.ch1VoltageRange]);
  else // if(channelMark == 1)
    str.sprintf("%.1lfV", offset / 2.5 * voltageRangeValue[(int)dso.ch2VoltageRange]);
  VoltageOffsetLabel->setText(str);
}

void ChannelSettingsWidget::switchButtonHandler()
{
    static bool state = true;

    state = !state;

    if (state == true)
        switchButton->setText("ON");
    else
        switchButton->setText("OFF");

    emit switchStateChanged(channelMark, state);
}

void ChannelSettingsWidget::couplingButtonHandler()
{
    DSOControlInfo dso;
    ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
    int coupling;

    if (channelMark == 0)
        coupling = (int)dso.ch1Coupling;
    else // if (channelMark == 1)
        coupling = (int)dso.ch2Coupling;

    switch (coupling)
    {
        case DSO_COUPLING_DC:
            coupling = DSO_COUPLING_AC;
            couplingButton->setText("AC");
            break;
        case DSO_COUPLING_AC:
            coupling = DSO_COUPLING_DC;
            couplingButton->setText("DC");
            break;
        default:
            break;
    }

    if (channelMark == 0)
        dso.ch1Coupling = coupling;
    else // if (channelMark == 1)
        dso.ch2Coupling = coupling;

    ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);

 //   emit couplingModeChanged(channelMark, status);
}

// 打开调色板，设置通道颜色
void ChannelSettingsWidget::markColorButtonHandler()
{
    // 打开系统调色板
    QColorDialog* m_pQColorDialog  = new QColorDialog;

    // 获取并设置用户设置的颜色
    setMarkColor(m_pQColorDialog->getColor());

    // 销毁对象
    delete m_pQColorDialog;
}
