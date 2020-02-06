#ifndef SAMPLERATESETTINGWIDGET_H
#define SAMPLERATESETTINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMoveEvent>

class SampleRateSettingWidget :  public QGroupBox
{
    Q_OBJECT
public:
    explicit SampleRateSettingWidget(QWidget *parent = 0);
    ~SampleRateSettingWidget();
    void setSampleRateDisplay(QString str);
    void setResolutionDisplay(QString str);

    void updateParameter();

signals:
    void sampleRateChanged(int);

public slots:

private:
    QWidget* parentWidget;
    QWidget* knobWidget;
    QLabel*  sampleRateLabel;
    QLabel*  resolutionLabel;
    int      knobCount;

    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void updateKnob(QWidget *widget);
};

#endif // SAMPLERATESETTINGWIDGET_H
