#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include "global.h"
#include "connectwindow.h"
#include "component/titlebarwidget.h"
#include "component/statusbarwidget.h"
#include "component/systembutton.h"

namespace Ui {
  class AboutWindow;
}

class AboutWindow : public QWidget
{
  Q_OBJECT

public:
  explicit AboutWindow(QWidget *parent = 0);
  ~AboutWindow();

public slots:
  void closeWindow();
  void updateSubtitle();

private:
  void paintEvent(QPaintEvent *);
  void initUI();
  void initLayout();

private:
  QWidget *parentWidget;
  Ui::AboutWindow *ui;
  TitleBarWidget *titleBarWidget;
  QLabel *gifLabel;
  QLabel *titleText;
  QLabel *text;
  QLabel *text1;
  QTimer *mTimer;
  QString subtitleString;
};

#endif // ABOUTWINDOW_H
