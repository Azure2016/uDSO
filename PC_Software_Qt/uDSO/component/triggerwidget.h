#ifndef TRIGGERWIDGET_H
#define TRIGGERWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QEvent>

class TriggerWidget : public QGroupBox
{
    Q_OBJECT
public:
    TriggerWidget(QWidget *parent = 0);
    ~TriggerWidget();    
    void updateParameter();

signals:
    void signal_channelChanged(int data);
    void signal_edgeChanged(int data);
    void signal_levelChanged(int data);

public slots:
    void channelComboBoxHandler(const QString & data);
    void edgeComboBoxHandler(const QString & data);
    void levelComboBoxHandler(const QString & data);

private:
    QWidget* parentWidget;

    QLabel* channelLabel;
    QLabel* edgeLabel;
    QLabel* levelLabel;

    QComboBox* channelComboBox;
    QComboBox* edgeComboBox;
    QComboBox* levelComboBox;
};

#endif // TRIGGERWIDGET_H
