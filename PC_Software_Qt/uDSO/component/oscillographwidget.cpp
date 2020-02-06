#include "oscillographwidget.h"
#include "window/mainwindow.h"

OscillographWidget::OscillographWidget(QWidget *parent, int number) :
  QWidget(parent)
{
    pWidget = parent;

    this->setMinimumSize(1012, 627);
    this->setAutoFillBackground(true);
    this->setCursor(QCursor(Qt::CrossCursor));

    // 更新界面标识为假
    updateWaveFlag = false;
    // 波形数据长度初始化为0
    waveDataLength = -1;
    // 波形数据指针初始化为空
    waveData = NULL;

    // 设置通道数
    channelNumber  = number;

    // 分配显示缓冲内存
    try
    {
        for (int i=0; i<channelNumber; i++)
        {
            dispBufferArray[i] = new QPoint[DISPLAY_BUFFER_MAX_LENGTH];
        }
    }
    catch (std::bad_alloc)
    {
        qDebug()<< "dispBufferArray[] bad_alloc";
    }

    // 初始化通道标识颜色
    for (int i=0; i<4; i++)
    {
        channelMarkColor[i] = Qt::blue;
    }
    // 设置通道是否使能
    for (int i=0; i<4; i++)
    {
        channelDisplayState[i] = true;
    }

}

OscillographWidget::~OscillographWidget()
{
    for (int i=0; i<channelNumber; i++)
    {
        delete[] dispBufferArray[i];
    }
}

// slot
void OscillographWidget::setChannelMarkColor(int chn, QColor color)
{
    channelMarkColor[chn] = color;
}

// slot
void OscillographWidget::setChannelDisplay(int chn, bool state)
{
    channelDisplayState[chn] = state;
}

// slot
void OscillographWidget::updateMode(int status)
{
    Mode = status;
    this->repaint();
}

//
void OscillographWidget::dispWaveData(unsigned char* data, int length)
{
    if (data != NULL)
    {
        updateWaveFlag = true;
        waveData       = data;
        waveDataLength = length;
    }

    this->repaint();
}

//
void OscillographWidget::paintEvent(QPaintEvent *)
{
    this->drawFrame();
    this->drawWave();
    this->displayMeasure();
}

// 绘制示波器背景
void OscillographWidget::drawFrame()
{
    double width, height;
    double dx, dy;

    QPainter painter(this);
    painter.fillRect(QRect(0,0,this->width(),this->height()),"#101010");

    width  = (double) this->width();
    height = (double) this->height();
    dx = width  / 10.0;
    dy = height / 8.0;

    painter.setPen(QPen(QColor(0x60,0x60,0x60),1,Qt::DotLine));
    for (double x=dx; x<width; x+=dx)
    {
        painter.drawLine((int)(x+0.5), 0, (int)(x+0.5), height);
    }
    for (double y=dy; y<height; y+=dy)
    {
        painter.drawLine(0, (int)(y+0.5), width, (int)(y+0.5));
    }
    painter.setPen(QPen(QColor(0x60,0x60,0x60),1,Qt::SolidLine));
    painter.drawLine((int)(dx*5+0.5), 0, (int)(dx*5+0.5), height);
    painter.drawLine(0, (int)(dy*4+0.5), width, (int)(dy*4+0.5));
}

// 绘制波形
void OscillographWidget::drawWave()
{
    int dispWidth  = this->width() ;
    int dispHeight = this->height();

    double cnt  = 0.0;
    double step = (double)dispWidth / (double)waveDataLength;
    QPainter painter(this);

    // 压缩显示波形
    for (int i=0; i<channelNumber; i++)
    {
        cnt = 0.0;

        if ( updateWaveFlag == true )
        {
            for (int j=0; j<waveDataLength; j++)
            {
                dispBufferArray[i][j].setX( int(cnt+0.5) );
                dispBufferArray[i][j].setY( (unsigned short)((1- *((waveData+i*SAMPLING_LENGTH_MAX)+j) / 256.0)*dispHeight ) );
                cnt += step;
            }
        }

        // 设置波形显示的画笔
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(channelMarkColor[i], 1, Qt::SolidLine));
        // 当通道被使能时，才显示波形
        if (channelDisplayState[i] == true)
            painter.drawPolyline(dispBufferArray[i], waveDataLength);
    }

    if (updateWaveFlag == true)
        updateWaveFlag = false;
}

// 显示测量参数
void OscillographWidget::displayMeasure()
{
  QPainter painter(this);
  QString  str;

  int startX  = 10;  //this->width() - 270 - 10;
  int startY  = 10;
  int mWidth  = 240;
  int mHeight = 140;

  // 绘制方框
  painter.setPen(QPen(QColor(0x60,0x60,0x60),1,Qt::SolidLine));
  painter.fillRect(QRect(startX, startY,mWidth,mHeight),QColor(0x60,0x60,0x60,200));
  painter.drawRect(startX,startY,mWidth,mHeight);

  //
  painter.setPen(Qt::red);
  painter.setFont(QFont("微软雅黑",10,QFont::Bold,false));
  switch(Mode)
  {
      case 0:
        str = "STATUS - RUN";
        break;
      case 1:
        str = "STATUS - STOP";
        break;
      case 2:
        str = "STATUS - SINGLE";
        break;
      case 3:
        str = "STATUS - AUTO";
        break;
      case 4:
        str = "STATUS - DEFAULT";
        break;
      default:
        break;
  }
  painter.drawText(QRect(startX + 5, startY + 5, 180, 20), str);

  //
  painter.setPen(Qt::white);
  painter.setFont(QFont("微软雅黑",10,QFont::Normal,false));
  //
  if (channelDisplayState[0])
  {
    str = "CH1";
    painter.drawText(QRect(startX + 5, startY + 5 + 20, 180, 20),str);
    painter.drawText(QRect(startX + 5, startY + 5 + 38, 180, 20),str);
    painter.drawText(QRect(startX + 5, startY + 5 + 56, 180, 20),str);

    double freq = avgFrequency(DSO_CHANNEL1, ((MainWindow *)pWidget)->usbThread.measureData[0][0]);
    if(freq >= 1000000.0)
        str.sprintf("频率：%.3lfMHz",  freq / 1000000.0);
    else if(freq >= 1000.0)
        str.sprintf("频率：%.3lfKHz",  freq / 1000.0);
    else if(freq == 0.0)
        str.sprintf("频率：No edge");
    else
        str.sprintf("频率：%.3lfHz", freq);
    painter.drawText(QRect(startX + 40, startY + 5 + 20, 180, 20),str);

    str.sprintf("峰峰值：%.2lf V", ((MainWindow *)pWidget)->usbThread.measureData[0][1]);
    painter.drawText(QRect(startX + 40, startY + 5 + 38, 180, 20),str);

    str.sprintf("有效值：%.2lf V", ((MainWindow *)pWidget)->usbThread.measureData[0][2]);
    painter.drawText(QRect(startX + 40, startY + 5 + 56, 180, 20),str);
  }
  //
  if (channelDisplayState[1])
  {
    str = "CH2";
    painter.drawText(QRect(startX + 5, startY + 5 + 74, 180, 20),str);
    painter.drawText(QRect(startX + 5, startY + 5 + 92, 180, 20),str);
    painter.drawText(QRect(startX + 5, startY + 5 + 110, 180, 20),str);

    double freq = avgFrequency(DSO_CHANNEL2, ((MainWindow *)pWidget)->usbThread.measureData[1][0]);
    if(freq >= 1000000.0)
        str.sprintf("频率：%.3lfMHz",  freq / 1000000.0);
    else if(freq >= 1000.0)
        str.sprintf("频率：%.3lfKHz",  freq / 1000.0);
    else if(freq == 0.0)
        str.sprintf("频率：No edge");
    else
        str.sprintf("频率：%.3lfHz", freq);
    painter.drawText(QRect(startX + 40, startY + 5 + 74, 180, 20),str);

    str.sprintf("峰峰值：%.2lf V", ((MainWindow *)pWidget)->usbThread.measureData[1][1]);
    painter.drawText(QRect(startX + 40, startY + 5 + 92, 180, 20),str);

    str.sprintf("有效值：%.2lf V", ((MainWindow *)pWidget)->usbThread.measureData[1][2]);
    painter.drawText(QRect(startX + 40, startY + 5 + 110, 180, 20),str);
  }

  // 触发线
  double width, height;
  double dy;
  width  = this->width();
  height = this->height();
  dy = height / 8.0;

  DSOControlInfo dso;
  ((MainWindow *)pWidget)->usbThread.getDSOParameter(dso);

  double dbTemp;
  double voltageRange;
  if (dso.trigChannel == DSO_CHANNEL1)
    voltageRange = dso.ch1VoltageRange;
  else
    voltageRange = dso.ch2VoltageRange;
  switch ((int)voltageRange)
  {
    case 0: dbTemp = 80/2.0; break;
    case 1: dbTemp = 40/2.0; break;
    case 2: dbTemp = 16/2.0; break;
    case 3: dbTemp = 8/2.0; break;
    case 4: dbTemp = 4/2.0; break;
    case 5: dbTemp = 1.6/2.0; break;
    case 6: dbTemp = 0.8/2.0; break;
    case 7: dbTemp = 0.4/2.0; break;
    case 8: dbTemp = 0.16/2.0; break;
    case 9: dbTemp = 0.04/2.0; break;
    default: break;
  }
  dbTemp /= height;
  dbTemp = (dso.trigLevel - 1.25) / dbTemp;
  painter.setPen(QPen(Qt::green,1,Qt::SolidLine));
  painter.drawLine(0, (int)(dy*4 - dbTemp + 0.5), width, (int)(dy*4 - dbTemp + 0.5));
  painter.drawPixmap(0, (int)(dy*4 - dbTemp + 0.5) - 6, QPixmap(":/img/osc/Trig.png"));

  painter.drawPixmap(width - 16, (int)(dy*4+0.5) - 3, QPixmap(":/img/osc/gnd3.png"));
}

double OscillographWidget::avgFrequency(int channel, double frequency)
{
  using namespace std;

  if (channel == DSO_CHANNEL1)
  {
      if (listFreq1.size() >= 20)
      {
          listFreq1.pop_front();
      }
      listFreq1.push_back(frequency);

      double sum = 0.0;
      for (list<double>::iterator it = listFreq1.begin(); it != listFreq1.end() ; it++)
      {
          sum += *it;
      }
      return sum / listFreq1.size();
  }
  else
  {
      if (listFreq2.size() >= 20)
      {
          listFreq2.pop_front();
      }
      listFreq2.push_back(frequency);

      double sum = 0.0;
      for (list<double>::iterator it = listFreq2.begin(); it != listFreq2.end() ; it++)
      {
          sum += *it;
      }
      return sum / listFreq2.size();
  }
}

// widget的鼠标滚轮事件
void OscillographWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {}
    else
    {}
}
