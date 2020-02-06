#ifndef USB_H
#define USB_H

#include <QMainWindow>
#include <QDebug>

#include "lusb0_usb.h"

class usb
{
public:
     usb();
    ~usb();

     bool connectDevice(void);
     bool unconnectDevice(void);
     bool getConnectStatus(void);
     bool sendData(unsigned char* data,    int length);
     bool receiveData(unsigned char* data, int length);

     void setDeviceVID(int vid);
     void setDevicePID(int pid);
     int  getDeviceVID();
     int  getDevicePID();

protected:

private:
     usb_dev_handle* open_device(void);
     bool isOpenConnected(void);

     int                deviceVID;
     int                devicePID;
     usb_dev_handle*    usb_device;
     struct usb_device* device;
};

#endif // USB_H
