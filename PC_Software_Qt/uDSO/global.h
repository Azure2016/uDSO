#ifndef GLOBAL
#define GLOBAL

#include <QString>
#include "usb/usb.h"

// 程序版本号
#define UDSO_VERSION "0.0.6"
// 程序更新日期
#define UDSO_UPDATE  "2017.01.01"
// 开发者
#define USDO_AUTHOR  "Azure"

// 窗口标题
#define MAIN_WINDOW_TITLE     "μDSO"
#define CONNECT_WINDOW_TITLE  "设备连接"
#define DDS_WINDOW_TITLE      "信号发生器"
#define SETTINGS_WINDOW_TITLE "全局设置"
#define ABOUT_WINDOW_TITLE    "关于"

// 设置是否有启动界面
#define ENABLE_SPLASH_SCREEN true

// 使能仿真模式
#define ENABLE_SIMULATION_MODE true

// 标题栏的默认高度
#define DEFAULT_TITLE_BAR_HEIGHT 30

// 状态栏的默认高度
#define DEFAULT_STATUS_BAR_HEIGHT 25

// 主窗口的默认大小
#define DEFAULT_MAIN_WINDOW_WIDTH  1200
#define DEFAULT_MAIN_WINDOW_HEIGHT 800

// “连接设备”窗口的默认大小
#define DEFAULT_CONNECT_WINDOW_WIDTH  320
#define DEFAULT_CONNECT_WINDOW_HEIGHT 280

// “信号发生器”窗口的默认大小
#define DEFAULT_DDS_WINDOW_WIDTH  320
#define DEFAULT_DDS_WINDOW_HEIGHT 200

// “全局设置”窗口的默认大小
#define DEFAULT_SETTINGS_WINDOW_WIDTH  640
#define DEFAULT_SETTINGS_WINDOW_HEIGHT 500

// “关于uDSO”窗口的默认大小
#define DEFAULT_ABOUT_WINDOW_WIDTH  640
#define DEFAULT_ABOUT_WINDOW_HEIGHT 420

// 采样长度
//#define SAMPLING_LENGTH_MAX       2048/*16384*/
#define SAMPLING_LENGTH_MAX       600
// 显示缓冲最大长度
#define DISPLAY_BUFFER_MAX_LENGTH 600

extern QString g_Platform;  // 当前平台
extern usb*    g_USBDevice; // USB设备
extern QString g_ApplicationDirPath;

// 控制命令
#define CONTROL_CMD_AD_DATA ((unsigned char) 0)
#define CONTROL_CMD_DSO     ((unsigned char) 1)
#define CONTROL_CMD_DDS     ((unsigned char) 2)

// DDS
////////////////////////////////////////////////////////////////////////////////
// 波形枚举
#define DDS_WAVE_SINE       0
#define DDS_WAVE_SQUA       1
#define DDS_WAVE_TRI        2
#define DDS_WAVE_RAMP       3
#define DDS_WAVE_NAMP       4
#define DDS_WAVE_SINC       5
#define DDS_WAVE_NOISE      6
#define DDS_WAVE_EXPR       7
#define DDS_WAVE_EXPF       8
#define DDS_WAVE_FULL_SIN_P 9
#define DDS_WAVE_FULL_SIN_N 10
#define DDS_WAVE_HALF_SIN_P 11
#define DDS_WAVE_HALF_SIN_N 12
#define DDS_WAVE_GAUSSIAN   13
#define DDS_WAVE_DC         14
#define DDS_WAVE_STA        15
#define DDS_WAVE_ARB        16
// DDS数据信号类型
typedef struct{
  int    wave;
  double frequency;
  double amplitude;
  double offset;
  double duty;
}DDSControlInfo;

// DSO
////////////////////////////////////////////////////////////////////////////////
#define DSO_COUPLING_DC 0
#define DSO_COUPLING_AC 1
#define DSO_TRIG_FALL   0
#define DSO_TRIG_RAISE  1
#define DSO_CHANNEL1 0
#define DSO_CHANNEL2 1

#define DSO_SAMPLE_RATE_NUM   17
#define DSO_VOLTAGE_RANGE_NUM 11

const char textSampleRate[DSO_SAMPLE_RATE_NUM][2][20] =
{
  {"100M/s", "100ms/div"},
  {"50M/s",  "50ms/div"},
  {"25M/s",  "20ms/div"},
  {"10M/s",  "10ms/div"},
  {"5M/s",   "5ms/div"},
  {"2.5M/s", "2ms/div"},
  {"1M/s",   "1ms/div"},
  {"500K/s", "500us/div"},
  {"250K/s", "200us/div"},
  {"100K/s", "100us/div"},
  {"50K/s",  "50us/div"},
  {"25K/s",  "20us/div"},
  {"10K/s",  "10us/div"},
  {"5K/s",   "5us/div"},
  {"2.5K/s", "2us/div"},
  {"1K/s",   "1us/div"},
  {"0.5K/s", "500ns/div"}
};
const char textVoltageRange[DSO_VOLTAGE_RANGE_NUM][2][20] =
{
  {"80V", "10V/div"},
  {"40V", "5V/div"},
  {"16V", "2V/div"},
  {"8V",  "1V/div"},
  {"4V",  "500mV/div"},
  {"1.6V", "200mV/div"},
  {"0.8V", "100mV/div"},
  {"0.4V", "50mV/div"},
  {"0.16V","20mV/div"},
  {"0.8V", "10mV/div"},
  {"0.04V","5mV/div"}
};
const double voltageRangeValue[DSO_VOLTAGE_RANGE_NUM] = {80, 40, 16, 8, 4, 1.6, 0.8, 0.4, 0.16, 0.08,0.04};

// DSO数据类型
typedef struct
{
    double sampleRate;

    double ch1Offset;
    double ch1VoltageRange;
    double ch1Coupling;

    double ch2Offset;
    double ch2VoltageRange;
    double ch2Coupling;

    double trigChannel;
    double trigEdge;
    double trigLevel;
}DSOControlInfo;


#endif // GLOBAL

