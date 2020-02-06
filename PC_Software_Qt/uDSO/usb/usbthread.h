#ifndef USBTHREAD_H
#define USBTHREAD_H

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QThread>
#include <QMutex>
#include "global.h"

class USBThread : public QThread
{
    Q_OBJECT
public:
    explicit USBThread(QObject* parent = 0);
    void stop();
    void setReadIndex(int index);

    // 设置DDS
    void setDDSParameter(DDSControlInfo para);
    void getDDSParameter(DDSControlInfo & para);
    // 设置DSO
    void setDSOParameter(DSOControlInfo para);
    void getDSOParameter(DSOControlInfo & para);

    // 接收数据缓存数组，乒乓数组、2通道、SAMPLING_LENGTH_MAX的长度
    unsigned char adData[2][2][SAMPLING_LENGTH_MAX];
    double measureData[2][3];

signals:
    void signal_ReviceFinish(int);     // 数据接收满信号

protected:
    void run();

private:
    void reviceData();

    void controlDDS();
    void controlDSO();
    void simulationWaveData();

    volatile int  writeIndex; // 要写入数据的数组
    volatile int  readIndex;  // 读取数据的缓冲区
    volatile bool stopped;    // 停止线程

    QMutex ddsMutex;
    bool ddsFlag;
    DDSControlInfo ddsControlInfo;

    QMutex dsoMutex;
    bool dsoFlag;
    DSOControlInfo dsoControlInfo;
};

#endif // USBTHREAD_H
