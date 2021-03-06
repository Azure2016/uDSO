## uDSO是什么？
这是一个基于Qt的虚拟示波器软件，通过USB2.0与采样设备进行通信，目前使用STM32F407VET作为采样设备主控芯片，STM32F407VET直接控制高速ADC(AD9280)进行采样，后面计划可能会改用其它芯片（例如ZYNQ等）。当前软件主要支持Windows平台，后面计划会支持移动平台（Android）...

![](Screenshot/demo.png)

## uDSO有什么功能？
- 示波器
- 逻辑分析仪
- 信号发生器
- ...

## uDSO能用来干什么？
- 便携的数字示波器，虚拟示波器
- 替代传统数字示波器
- ...

## 关于 uDSO
* 这是一个长期开发和更新的项目，不会有明确的更新时间和终止期限，uDSO 开源、自由、跨平台（Linux/Windows）！
* 公布所有源代码，以及采样设备的原理图和PCB文件（假如这些东西都能被公开的话）...

#### 软件运行截图
Windows10 系统
![image 运行截图1](Screenshot/20160119223711.png)
Windows10 系统
![image 运行截图2](Screenshot/20160119223728.png)

#### 硬件电路效果图
主控板电路效果图
![image 电路效果图](Screenshot/主控电路效果图.png)

## TODO
### 现有问题：
- 还没有完成第一版程序，源代码未上传公开
- 还没有完成第一版电路设计，硬件还未开始调试，PCB未上传公开，硬件是短腿
- 开始时没有明确的目标，本人也比较拖拉，导致整个项目进展非常缓慢 :P
- ...

### 待添加功能：
- 赶紧完成第一版程序，将源代码上传公开！！！
- 赶紧完成第一版电路，将PCB上传公开！！！
- ...

## License
GNU GENERAL PUBLIC LICENSE Version 3

## 关于作者
![image AzureIcon](Screenshot/Azure.png)

E-mail: 985424805@qq.com（欢迎联系交流）
