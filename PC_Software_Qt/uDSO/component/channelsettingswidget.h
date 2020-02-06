#ifndef CHANNELSETTINGSWIDGET_H
#define CHANNELSETTINGSWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QString>
#include <QLabel>
#include <QEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QColorDialog>
#include "systembutton.h"

class ChannelSettingsWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ChannelSettingsWidget(QWidget *parent = 0, int mark = 0, QString str = "", QColor color = Qt::blue);
    ~ChannelSettingsWidget();
    void   setMarkColor(QColor color);
    void   setVoltageOffsetDisplay(QString str);
    void   setVoltageRangeDisplay(QString str);
    QColor getMarkColor();

    void updateParameter();

public slots:
    void switchButtonHandler();
    void couplingButtonHandler();
    void markColorButtonHandler();

signals:
    void switchStateChanged(int, bool);
    void voltageOffsetChanged(int, int);
    void voltageRangeChanged(int, int);
    void couplingModeChanged(int, int);
    void markColoChange(int, QColor);

protected:

private:
    QWidget* parentWidget;
    QWidget* knob1;
    QWidget* knob2;
    QLabel*  voltageRangeLabel;
    QLabel*  VoltageOffsetLabel;
    QLabel*  markColorLabel;

    SystemButton* switchButton;
    SystemButton* markColorButton;
    SystemButton* couplingButton;

    int knob1Count;
    int knob2Count;

    // 面板控制的参数
    int    channelMark;
    int    voltageRange;
    int    voltageOffset;
    int    couplingMode;
    bool   channelEnable;
    QColor markColor;

    void mousePressEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void updateKnob(QWidget* widget);
};

#endif // CHANNELSETTINGSWIDGET_H
