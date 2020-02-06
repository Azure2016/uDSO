#include "systembutton.h"
#include <QFont>

SystemButton::SystemButton(QWidget *parent, QString str):
  QPushButton(parent)
{
    this->setFixedSize(75,25);
    this->setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet("QPushButton{ "
                      "color: rgb(0, 0, 0);  "
                      "background-color: rgb(255, 255, 255); "
                      "border: 1px solid gray; "
                      "border-radius: 0px; "
                      "text-align : center;}"

                      "QPushButton::hover{ "
                      "background-color: rgb(229, 241, 251); "
                      "border: 1px solid rgb(0, 120, 215); }"

                      "QPushButton::pressed{ "
                      "color: white; "
                      "background-color: rgb(0, 99, 177); "
                      "border: 1px solid rgb(0, 84, 153);}"

                      "QPushButton::disabled{ "
                      "color: rgb(114, 114, 114); "
                      "background-color: rgb(220, 220, 220); "
                      "border: 1px solid gray; }");
    this->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    this->setText(str);
}

SystemButton::~SystemButton()
{

}

