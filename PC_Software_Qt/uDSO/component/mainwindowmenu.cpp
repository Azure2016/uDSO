#include "mainwindowmenu.h"

MainWindowMenu::MainWindowMenu(QWidget *parent):
  QMenu(parent)
{
    parentWindow = parent;

    this->setStyleSheet("QMenu{  background-color:white; border: 1px solid #0063B1; }"
                        "QMenu::item:selected{ background-color: rgb(234, 243, 253); color: black;} ");

    connectAction  = new QAction(this);
    ddsAction      = new QAction(this);
    settingsAction = new QAction(this);
    aboutAction    = new QAction(this);

    connectAction->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    ddsAction->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    settingsAction->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    aboutAction->setFont(QFont("微软雅黑", 10, QFont::Normal, false));

    connectAction->setText(CONNECT_WINDOW_TITLE);
    ddsAction->setText(DDS_WINDOW_TITLE);
    settingsAction->setText("全局设置");
    aboutAction->setText(ABOUT_WINDOW_TITLE);

    this->addAction(connectAction);
    this->addAction(ddsAction);
 //   this->addAction(settingsAction);
    this->addAction(aboutAction);
}

MainWindowMenu::~MainWindowMenu()
{

}

void MainWindowMenu::intConnect()
{
  connect(connectAction, SIGNAL(triggered()), parentWindow, SLOT(openConnectWindow()) );
  connect(aboutAction,   SIGNAL(triggered()), parentWindow, SLOT(openAboutWindow())   );
  connect(ddsAction,     SIGNAL(triggered()), parentWindow, SLOT(openDDSWindow())     );
}
