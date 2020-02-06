#include "usb.h"
#include "lusb0_usb.h"

//-----------------------define------------------//
#define EP_IN  0x81
#define EP_OUT 0x01

//-----------------------variable----------------//
static unsigned long int counter = 0;
static int search_flag = 0;
static int pain_flag = 1;
static int open_flag = 0;

//-----------------------function----------------//
usb::usb()
{
    deviceVID = 0x0483;
    devicePID = 0x5710;
}

usb::~usb()
{
    if(open_flag == 1)
    {
        usb_close(usb_device);
    }
}

bool usb::connectDevice(void)
{
    if (open_flag == 1)
    {
        return true;
    }
    else
    {
        usb_init();
        usb_find_busses();
        usb_find_devices();
        usb_device = open_device();
        if(search_flag == 1)
        {
            open_flag = 0;
            search_flag = 0;
            qDebug()<< "Device opened is error"<< endl;
            return false;
        }
        open_flag = 1;
        usb_set_configuration(usb_device,1);
        usb_claim_interface(usb_device,0);
        return true;
    }
}

bool usb::unconnectDevice(void)
{
    if (open_flag == 1)
    {
        usb_close(usb_device);
        open_flag = 0;
        pain_flag = 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool usb::getConnectStatus(void)
{
    return (bool)open_flag;
}

usb_dev_handle* usb::open_device(void)
{
    struct usb_bus *bus;

    for(bus = usb_get_busses(); bus; bus = bus->next)
    {
        for(device = bus->devices; device; device = device->next)
        {
            if(device->descriptor.idVendor == deviceVID && device->descriptor.idProduct == devicePID )
            {
                return usb_open(device);
            }

        }
    }

    search_flag = 1;

    return nullptr;
}

bool usb::sendData(unsigned char* data,    int length)
{
    if (open_flag == 0)
        return false;

    int res;

    // 发送命令数据
    res = usb_bulk_write(usb_device, EP_OUT, (char*)data, length, 2000);
    if(res < 0)
    {
        qDebug("error write: %s",usb_strerror());
        return false;
    }

    return true;
}

bool usb::receiveData(unsigned char* data, int length)
{
    if (open_flag == 0)
        return false;

    int res;

    // 接收数据
    res = usb_bulk_read(usb_device, EP_IN, (char*)data, length, 2000);
    if(res != length)
    {
        qDebug("packet_size error: %d, %d", res, length);
        return false;
    }

    return true;
}

void usb::setDeviceVID(int vid)
{
    deviceVID = vid;
}

void usb::setDevicePID(int pid)
{
    devicePID = pid;
}

int  usb::getDeviceVID()
{
    return deviceVID;
}

int  usb::getDevicePID()
{
    return devicePID;
}
