#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QLineEdit>
#include "global.h"
#include "component/titlebarwidget.h"
#include "component/systembutton.h"

namespace Ui {
  class ConnectWindow;
}

class ConnectWindow : public QWidget
{
  Q_OBJECT

public:
  explicit ConnectWindow(QWidget *parent = 0);
  ~ConnectWindow();

signals:
    void signal_Connected(QString);
    void signal_Unconnected(QString);

public slots:
  void closeWindow();
  void connectDevice();
  void unconnectDevice();

private:
  void paintEvent(QPaintEvent *);
  void initUI();
  void initLayout();

private:
  Ui::ConnectWindow* ui;
  TitleBarWidget*    titleBarWidget;
  QLineEdit*         pidLineEdit;
  QLineEdit*         vidLineEdit;
  QLabel*            pidTextLabel;
  QLabel*            vidTextLabel;
  QLabel*            imageLabel;
  SystemButton*      connectDeviceButton;
  SystemButton*      unconnectDeviceButton;
};

#endif // CONNECTWINDOW_H
