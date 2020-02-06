#include "sampleratesettingwidget.h"
#include "window/mainwindow.h"

SampleRateSettingWidget::SampleRateSettingWidget(QWidget *parent) :
     QGroupBox(parent)
{
    parentWidget = parent;

    this->setFixedSize(160,95);
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
    this->setTitle("采样速率");

    // 旋钮
    knobWidget = new QWidget(this);
    knobCount  = 0;
    knobWidget->setFixedSize(64, 64);
    knobWidget->setStyleSheet("border-image: url(:/img/knob/knob0.png);");
    knobWidget->move(95,20);

    // 标签
    sampleRateLabel = new QLabel(this);
    sampleRateLabel->setFrameShape(QFrame::Box);
    sampleRateLabel->setStyleSheet("QLabel { color: blue; border: 1px solid  gray;  }");
    sampleRateLabel->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    sampleRateLabel->setText("100M/s");
    sampleRateLabel->setGeometry(10, knobWidget->y()+5, 80, 25);
    sampleRateLabel->setAlignment(Qt::AlignCenter);

    // 标签
    resolutionLabel = new QLabel(this);
    resolutionLabel->setFrameShape(QFrame::Box);
    resolutionLabel->setStyleSheet("QLabel { color: blue;  border: 1px solid  gray;}");
    resolutionLabel->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    resolutionLabel->setText("10us/Div");
    resolutionLabel->setGeometry(10, sampleRateLabel->y()+30, 80, 25);
    resolutionLabel->setAlignment(Qt::AlignCenter);

    updateParameter();
}

SampleRateSettingWidget::~SampleRateSettingWidget()
{

}

void SampleRateSettingWidget::setSampleRateDisplay(QString str)
{
    sampleRateLabel->setText(str);
}

void SampleRateSettingWidget::setResolutionDisplay(QString str)
{
    resolutionLabel->setText(str);
}

void SampleRateSettingWidget::updateParameter()
{
  DSOControlInfo dso;
  ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
  sampleRateLabel->setText(textSampleRate[(int)dso.sampleRate][0]);
  resolutionLabel->setText(textSampleRate[(int)dso.sampleRate][1]);
}

void SampleRateSettingWidget::mousePressEvent(QMouseEvent *event)
{
    if (   event->x() >= knobWidget->x()
        && event->x() <= knobWidget->x() + knobWidget->width()
        && event->y() >= knobWidget->y()
        && event->y() <= knobWidget->y() + knobWidget->height()
        )
      {
        DSOControlInfo dso;
        ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
        int rate = (int)dso.sampleRate;

        if (event->button() == Qt::LeftButton)
        {
          knobCount = (knobCount+1)%8;
          rate = (rate <= 0) ? 0 : (rate-1);
          emit sampleRateChanged(1);
        }
        else if (event->button() == Qt::RightButton)
        {
          knobCount = knobCount <= 0 ? 7 : knobCount-1;
          rate = (rate >= (DSO_SAMPLE_RATE_NUM - 1)) ? (DSO_SAMPLE_RATE_NUM - 1) : (rate+1);
          emit sampleRateChanged(-1);
        }

        dso.sampleRate = rate;
        ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);
        sampleRateLabel->setText(textSampleRate[rate][0]);
        resolutionLabel->setText(textSampleRate[rate][1]);

        updateKnob(knobWidget);
      }
}

void SampleRateSettingWidget::wheelEvent(QWheelEvent *event)
{
    if (event->x() >= knobWidget->x()
        && event->x() <= knobWidget->x() + knobWidget->width()
        && event->y() >= knobWidget->y()
        && event->y() <= knobWidget->y() + knobWidget->height()
        )
    {
        DSOControlInfo dso;
        ((MainWindow *)parentWidget)->usbThread.getDSOParameter(dso);
        int rate = (int)dso.sampleRate;

        if (event->delta() > 0)
        {
          knobCount = ( knobCount+1 ) % 8;
          rate = (rate <= 0) ? 0 : (rate-1);
          emit sampleRateChanged(1);
        }
        else
        {
          knobCount = knobCount <= 0 ? 7 : knobCount-1;
          rate = (rate >= (DSO_SAMPLE_RATE_NUM - 1)) ? (DSO_SAMPLE_RATE_NUM - 1) : (rate+1);
          emit sampleRateChanged(-1);
        }

        dso.sampleRate = rate;
        ((MainWindow *)parentWidget)->usbThread.setDSOParameter(dso);
        sampleRateLabel->setText(textSampleRate[rate][0]);
        resolutionLabel->setText(textSampleRate[rate][1]);

        // 更新滚轮状态
        updateKnob(knobWidget);
      }
}

void SampleRateSettingWidget::updateKnob(QWidget *widget)
{
    QString str;

    if (widget == knobWidget)
      str.sprintf("border-image: url(:/img/knob/knob%d.png);",knobCount);

    widget->setStyleSheet(str);
}
