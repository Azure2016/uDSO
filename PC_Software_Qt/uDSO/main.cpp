#include "window/mainwindow.h"

int main(int argc, char *argv[])
{
    // 获取当前平台
#ifdef Q_OS_LINUX
    g_Platform = "LINUX";
#elif defined Q_OS_WIN
    g_Platform = "WINDOWS";
#endif

    // 创建Qt应用
    QApplication a(argc, argv);

    g_ApplicationDirPath = a.applicationDirPath();

    // 启动界面
#if ENABLE_SPLASH_SCREEN
    QPixmap pixmap(":/img/etc/splashscreen.png");
    QSplashScreen screen(pixmap);
    screen.show();
    QDateTime n=QDateTime::currentDateTime();
    QDateTime now;
    do
     {
        now=QDateTime::currentDateTime();
        a.processEvents();
     }
    while (n.secsTo(now) <= 2); //2s
#endif

    // 启动主界面
    MainWindow w;
    w.show();

    // 关闭启动画面
#if ENABLE_SPLASH_SCREEN
    screen.finish(&w);
#endif

    // 等待应用退出
    return a.exec();
}
