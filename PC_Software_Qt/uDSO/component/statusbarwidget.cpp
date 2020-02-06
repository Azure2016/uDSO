#include "statusbarwidget.h"
#include <QPalette>
#include <QDateTime>
#include <QLayout>

StatusBarWidget::StatusBarWidget(
    QWidget *parent,
    int height) :
  QWidget(parent)
{
    parent_widget = parent;

    isShowTime = false;

    //  this->setFixedSize(parent_widget->width(), height);
    this->setMinimumSize(parent_widget->width(), height);
    this->setFixedHeight(height);

    // 设置状态栏背景颜色
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,"#0063B1");
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    // 信息显示区
    message_label = new QLabel(this);
    message_label->setMinimumSize(600, this->height());
    message_label->setStyleSheet("QLabel{color:rgb(255, 255, 255);}");
    message_label->setFont(QFont("微软雅黑", 10, QFont::Normal, false));

    // 时间显示区
    time_label = new QLabel(this);
    time_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    time_label->setMinimumSize(150, this->height());
    time_label->setStyleSheet("QLabel{color:rgb(255, 255, 255);}");
    time_label->setFont(QFont("微软雅黑", 10, QFont::Normal, false));

    // 布局
    QHBoxLayout *layout =new QHBoxLayout();
    QSpacerItem *hSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addWidget(message_label);
    layout->addSpacerItem(hSpacer);
    layout->addWidget(time_label);
    layout->setContentsMargins(5, 0, 5, 0);
    this->setLayout(layout);

    // 设置显示时间的定时器
    show_time_timer = new QTimer(this);
    connect(show_time_timer, SIGNAL(timeout()), this, SLOT(showTime()) );
}

StatusBarWidget::~StatusBarWidget()
{

}

void StatusBarWidget::isEnabledShowTime(bool flag)
{
    if (flag == true && isShowTime != true)
        show_time_timer->start(500);
    else if (flag == false && isShowTime != false)
        show_time_timer->stop();
    else
    {}

    isShowTime = flag;
}

void StatusBarWidget::setWidth(int width)
{
    this->setFixedSize(width, this->height());
    //  message_label->setGeometry(5, 0, 600, this->height());
    time_label->setGeometry(width-155, 0, 150, this->height());
}

void StatusBarWidget::showMessage(QString str)
{
    message_label->setText(str);
}

void StatusBarWidget::showTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy.MM.dd hh:mm:ss");
    time_label->setText(str);
}

