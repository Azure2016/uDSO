#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QMouseEvent>

class TitleBarWidget : public QWidget
{
  Q_OBJECT
public:
    explicit TitleBarWidget(QWidget *parent = 0,
                            QString str = "",
                            int height = 30,
                            bool is_show_all_button = true);
    ~TitleBarWidget();
    void initConnect();
    void setWidth(int width);

signals:
    void closeWindow();

public slots:
    void maximizeWindow();
    void restoreWindow();

protected:

private:
    QWidget *parent_widget;
    QLabel *icon;
    QLabel *title_text;
    QPushButton *menu_button;
    QPushButton *min_button;
    QPushButton *max_button;
    QPushButton *restore_button;
    QPushButton *close_button;
    bool isShowAllButton;
    bool mouse_pressed;
    bool window_maximize;
    QPoint drag_position;

    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // TITLEBARWIDGET_H
