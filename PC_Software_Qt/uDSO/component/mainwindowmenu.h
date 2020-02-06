#ifndef MAINWINDOWMENU_H
#define MAINWINDOWMENU_H

#include <QWidget>
#include <QMenu>
#include "global.h"

class MainWindowMenu : public QMenu
{
public:
  explicit MainWindowMenu(QWidget *parent = 0);
  ~MainWindowMenu();
  void intConnect();

private:
  QWidget *parentWindow;
  QAction *connectAction;
  QAction *ddsAction;
  QAction *settingsAction;
  QAction *aboutAction;
};

#endif // MAINWINDOWMENU_H
