#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QTimer>

class StatusBarWidget : public QWidget
{
  Q_OBJECT
public:
  explicit StatusBarWidget(QWidget *parent = 0,int height = 25);
  ~StatusBarWidget();
  void isEnabledShowTime(bool flag);
  void setWidth(int width);
  void showMessage(QString str);

signals:

public slots:
  void showTime();

private:
  QWidget* parent_widget;
  bool     isShowTime;
  QLabel*  message_label;
  QLabel*  time_label;
  QTimer*  show_time_timer;
};

#endif // STATUSBARWIDGET_H
