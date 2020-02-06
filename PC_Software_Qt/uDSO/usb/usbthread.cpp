#include "usbthread.h"

#include <QDebug>
#include <QString>

#include "global.h"
#include "usb/usb.h"
#include <stdlib.h>
#include <cmath>

USBThread::USBThread(QObject* parent):
    QThread(parent)
{
    writeIndex = 0;
    readIndex  = -1;
    stopped    = false;

    // 清零测量参数
    for (int i = 0; i < 3; i++)
    {
        measureData[0][i] = 0.0;
        measureData[1][i] = 0.0;
    }

    // DDS参数
    ddsFlag = true;
    ddsControlInfo.wave      = DDS_WAVE_SINE;
    ddsControlInfo.frequency = 1000.0;
    ddsControlInfo.amplitude = 5.0;
    ddsControlInfo.offset    = 0.0;
    ddsControlInfo.duty      = 50.0;
    // DSO参数
    dsoFlag = true;
    dsoControlInfo.sampleRate = 8;
    dsoControlInfo.ch1Offset = 0.0;
    dsoControlInfo.ch1VoltageRange = 2;
    dsoControlInfo.ch1Coupling = DSO_COUPLING_DC;
    dsoControlInfo.ch2Offset =  0.0;
    dsoControlInfo.ch2VoltageRange = 2;
    dsoControlInfo.ch2Coupling = DSO_COUPLING_DC;
    dsoControlInfo.trigChannel = DSO_CHANNEL1;
    dsoControlInfo.trigEdge = DSO_TRIG_RAISE;
    dsoControlInfo.trigLevel = 1.25;
}

void USBThread::setDDSParameter(DDSControlInfo para)
{
    ddsMutex.lock();
    ddsFlag = true;
    ddsControlInfo = para;
    ddsMutex.unlock();
}

void USBThread::getDDSParameter(DDSControlInfo & para)
{
    para = ddsControlInfo;
}

void USBThread::setDSOParameter(DSOControlInfo para)
{
    dsoMutex.lock();
    dsoFlag = true;
    dsoControlInfo = para;
    dsoMutex.unlock();
}

void USBThread::getDSOParameter(DSOControlInfo & para)
{
    para = dsoControlInfo;
}

// 设置外部读缓冲的数组
void USBThread::setReadIndex(int index)
{
    readIndex = index;
}

void USBThread::run()
{
    while (stopped == false)
    {
        if (ENABLE_SIMULATION_MODE)
        {
            simulationWaveData();
            msleep(10);
        }
        else
        {
            // 检查设备是否连接
            if (g_USBDevice->getConnectStatus() == false)
            {
                qDebug()<< "设备未连接！";
                msleep(500);
                continue;
            }

            // 控制DDS
            controlDDS();

            // 控制DSO
            controlDSO();
        }
    }

    stopped = false;
}

void USBThread::stop()
{
    stopped = true;

    // 终止线程
    this->terminate();
}

void USBThread::controlDDS()
{
    if (ddsFlag)
    {
        double para[5];
        ddsMutex.lock();
        para[0] = ddsControlInfo.wave;
        para[1] = ddsControlInfo.frequency;
        para[2] = ddsControlInfo.amplitude;
        para[3] = ddsControlInfo.offset;
        para[4] = ddsControlInfo.duty;
        ddsFlag = false;
        ddsMutex.unlock();

        unsigned char cmd[41];
        cmd[0] = CONTROL_CMD_DDS;
        for (int i = 0; i < 40; i++)
        {
            cmd[i+1] = ((unsigned char *)para)[i];
        }
        if (g_USBDevice->sendData(cmd, sizeof(cmd)) == false)
        {
            qDebug()<< "DDS设置向USB设备发送命令失败！";
            return;
        }

        unsigned char retData[64];
        memset(retData, 0, sizeof(retData));
        if (g_USBDevice->receiveData(retData, 64) == false )
        {
            qDebug("DDS设置异常，回读信号失败");
            return;
        }
        if (retData[0] != 1)
        {
            qDebug("DDS设置异常，回读信号错误");
            return;
        }
    }
}

void USBThread::controlDSO()
{
    if (dsoFlag)
    {
        double para[10];
        dsoMutex.lock();
        para[0] = dsoControlInfo.sampleRate;
        para[1] = dsoControlInfo.ch1Offset;
        para[2] = dsoControlInfo.ch1VoltageRange;
        para[3] = dsoControlInfo.ch1Coupling;
        para[4] = dsoControlInfo.ch2Offset;
        para[5] = dsoControlInfo.ch2VoltageRange;
        para[6] = dsoControlInfo.ch2Coupling;
        para[7] = dsoControlInfo.trigChannel;
        para[8] = dsoControlInfo.trigEdge;
        para[9] = dsoControlInfo.trigLevel;
        dsoFlag = false;
        dsoMutex.unlock();

        unsigned char cmd[81];
        cmd[0] = CONTROL_CMD_DSO;
        for (int i = 0; i < 80; i++)
        {
            cmd[i+1] = ((unsigned char *)para)[i];
        }
        if (g_USBDevice->sendData(cmd, sizeof(cmd)) == false)
        {
            qDebug()<< "DSO设置向USB设备发送命令失败！";
            goto DSO_READ_AD_DATA;
        }

        unsigned char retData[64];
        memset(retData, 0, sizeof(retData));
        if (g_USBDevice->receiveData(retData, 64) == false )
        {
            qDebug("DSO设置异常，回读信号失败");
            goto DSO_READ_AD_DATA;
        }
        if (retData[0] != 1)
        {
            qDebug("DSO设置异常，回读信号错误");
            goto DSO_READ_AD_DATA;
        }
    }

DSO_READ_AD_DATA:
    // 判断是否可以开始读写数据
    if ( writeIndex == readIndex )
    {
        qDebug()<< "等待主界面处理完成数据";
        msleep(10);
        return;
    }

    // 向USB设备发送命令
    unsigned char cmd = CONTROL_CMD_AD_DATA;
    if (g_USBDevice->sendData(&cmd, 1) == false)
    {
        qDebug()<< "向USB设备发送命令失败！";
        return;
    }

    // 接收USB设备上传的数据
    if (g_USBDevice->receiveData(adData[writeIndex][0], SAMPLING_LENGTH_MAX) == false )
    {
        qDebug("adData[writeIndex][0] 数据接收异常");
    }
    if (g_USBDevice->receiveData(adData[writeIndex][1], SAMPLING_LENGTH_MAX)  == false )
    {
        qDebug("adData[writeIndex][1] 数据接收异常");
    }

    // 读取测量数据
    unsigned char buffer[48];
    if (g_USBDevice->receiveData(buffer, 48)  == false )
    {
        qDebug("measureData 数据接收异常");
    }
    for (int i = 0; i < 3; i++)
    {
        measureData[0][i] = *((double *)&buffer[i*8]);
        measureData[1][i] = *((double *)&buffer[i*8 + 24]);
    }

    // 发射接收完成信号
    emit signal_ReviceFinish(writeIndex);

    // 更新写的缓冲区
    writeIndex = (writeIndex+1)%2;
}

void USBThread::simulationWaveData()
{
    // 判断是否可以开始读写数据
    if ( writeIndex == readIndex )
    {
        qDebug()<< "等待主界面处理完成数据";
        msleep(10);
        return;
    }

    // 生成仿真数据
    for (int i = 0; i < SAMPLING_LENGTH_MAX; i++)
    {
        adData[writeIndex][0][i] = (sin((double)i / SAMPLING_LENGTH_MAX * 3 * 3.14 * 2) + 1) / 2.0 * 250 + rand() % 5;
    }
    for (int i = 0; i < SAMPLING_LENGTH_MAX; i++)
    {
        adData[writeIndex][1][i] = (sin((double)i / SAMPLING_LENGTH_MAX * 60 * 3.14 * 2) * cos((double)i / SAMPLING_LENGTH_MAX * 2 * 3.14 * 2) + 1) / 2.0 * 250 + rand() % 5;
    }

    // 发射接收完成信号
    emit signal_ReviceFinish(writeIndex);

    // 更新写的缓冲区
    writeIndex = (writeIndex+1)%2;
}
