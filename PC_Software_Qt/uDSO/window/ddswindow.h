#ifndef DDSWINDOW_H
#define DDSWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPainter>
#include <QComboBox>
#include <QLineEdit>
#include "global.h"
#include "component/titlebarwidget.h"
#include "component/systembutton.h"

namespace Ui {
class DDSWindow;
}

class DDSWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DDSWindow(QWidget *parent = 0);
    ~DDSWindow();

  void updateParameter();

public slots:
    void closeWindow();
    void setButtonHandler();

private:
    QWidget *parentWidget;

    Ui::DDSWindow   *ui;
    TitleBarWidget  *titleBarWidget;

    QLabel*       waveLabel;
    QLabel*       freqLabel;
    QLabel*       amplitudeLabel;
    QLabel*       offsetLabel;
    QComboBox*    waveComboBox;
    QLineEdit*    freqLineEdit;
    QLineEdit*    amplitudeLineEdit;
    QLineEdit*    offsetLineEdit;
    SystemButton* setButton;
    SystemButton* enableButton;


    void initUI();
    void initLayout();
    void paintEvent(QPaintEvent *);
};

#endif // DDSWINDOW_H
