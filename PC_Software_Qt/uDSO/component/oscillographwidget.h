#ifndef UDSOOSCILLOGRAPHWIDGET_H
#define UDSOOSCILLOGRAPHWIDGET_H

#include "global.h"
#include <QWidget>
#include <QTimer>
#include <QWheelEvent>
#include <QPainter>
#include <QRect>
#include <QPen>
#include <QDebug>
#include <QString>
#include <QLabel>

#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <list>


class OscillographWidget : public QWidget
{
  Q_OBJECT
public:
  explicit OscillographWidget(QWidget *parent = 0, int number = 0);
  ~OscillographWidget();
  void setWidgetSize(int height, int width);
  void dispWaveData(unsigned char* data, int length);

public slots:
  void setChannelMarkColor(int chn, QColor color); // channelsettings 通过信号槽直接关联
  void setChannelDisplay(int chn, bool state);     // channelsettings 直接
  void updateMode(int status);

protected:
  void wheelEvent(QWheelEvent *event);

private:
  QWidget*       pWidget;

  int            channelNumber;
  bool           updateWaveFlag;
  unsigned char* waveData;
  int            waveDataLength;
  QPoint*        dispBufferArray[4];

  // 测量参数
  long int freqValue;
  double   vppValue;
  double   vrmsValue;

  // 示波器状态
  int Mode;

  // 最多支持4通道
  QColor channelMarkColor[4];
  bool   channelDisplayState[4];

  void paintEvent(QPaintEvent *);
  void drawFrame();
  void drawWave();
  void displayMeasure();

  std::list<double> listFreq1;
  std::list<double> listFreq2;
  double avgFrequency(int channel, double frequency);
};

#endif // UDSOOSCILLOGRAPHWIDGET_H
