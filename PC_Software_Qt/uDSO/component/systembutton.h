#ifndef SYSTEMBUTTON_H
#define SYSTEMBUTTON_H

#include <QPushButton>
#include <QString>

class SystemButton : public QPushButton
{
  Q_OBJECT
public:
  explicit SystemButton(QWidget *parent = 0, QString str="");
  virtual ~SystemButton();
};

#endif // SYSTEMBUTTON_H
