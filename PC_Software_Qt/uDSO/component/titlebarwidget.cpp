#include "titlebarwidget.h"
#include <QLayout>

TitleBarWidget::TitleBarWidget(
      QWidget *parent,
      QString str,
      int height,
      bool is_show_all_button) :
    QWidget(parent)
{
    parent_widget   = parent;

    isShowAllButton = is_show_all_button;
    mouse_pressed   = false;
    window_maximize = false;

    this->setMinimumSize(parent_widget->width(), height);
    this->setFixedHeight(height);

    // 设置标题背景颜色
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,"#0063B1");
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    // 设置标题栏图标
    icon = new QLabel(this);
    icon->setMinimumSize(this->height(),  this->height());
//    icon->setStyleSheet("QLabel{border-image: url(:/img/etc/logo_border.png); }");
    icon->setStyleSheet("QLabel{border-image: url(:/img/etc/logo_border.png); }");
    // 设置标题文字
    title_text = new QLabel(this);
    title_text->setMinimumSize(100, this->height());
    title_text->setStyleSheet("QLabel{color:rgb(255, 255, 255);}");
    title_text->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    title_text->setText(tr(" ") + str);

    // 设置按钮
    if (isShowAllButton == true)
    {
        // 创建PushButton
        menu_button = new QPushButton(this);
        min_button = new QPushButton(this);
        max_button = new QPushButton(this);
        restore_button = new QPushButton(this);
        close_button = new QPushButton(this);
        // 设置PushButtond的大小
        menu_button->setFixedSize(46,this->height());
        min_button->setFixedSize(46,this->height());
        max_button->setFixedSize(46,this->height());
        restore_button->setFixedSize(46,this->height());
        close_button->setFixedSize(46,this->height());
        // 设置PushButton的样式
        menu_button->setStyleSheet("QPushButton{border-image: url(:/img/button/menu_normal.png);}"
                                 "QPushButton:hover{border-image: url(:/img/button/menu_hover.png);}"
                                 "QPushButton:pressed{border-image: url(:/img/button/menu_pressed.png);}");
        min_button->setStyleSheet("QPushButton{border-image: url(:/img/button/min_normal.png);}"
                                "QPushButton:hover{border-image: url(:/img/button/min_hover.png);}"
                                "QPushButton:pressed{border-image: url(:/img/button/min_pressed.png);}");
        max_button->setStyleSheet("QPushButton{border-image: url(:/img/button/max_normal.png);}"
                                "QPushButton:hover{border-image: url(:/img/button/max_hover.png);}"
                                "QPushButton:pressed{border-image: url(:/img/button/max_pressed.png);}");
        restore_button->setStyleSheet("QPushButton{border-image: url(:/img/button/restore_normal.png);}"
                                    "QPushButton:hover{border-image: url(:/img/button/restore_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/img/button/restore_pressed.png);}");
        close_button->setStyleSheet("QPushButton{border-image: url(:/img/button/close_normal.png);}"
                                  "QPushButton:hover{border-image: url(:/img/button/close_hover.png);}"
                                  "QPushButton:pressed{border-image: url(:/img/button/close_pressed.png);}");
        // PushButton无焦点
        menu_button->setFocusPolicy(Qt::NoFocus);
        min_button->setFocusPolicy(Qt::NoFocus);
        max_button->setFocusPolicy(Qt::NoFocus);
        restore_button->setFocusPolicy(Qt::NoFocus);
        close_button->setFocusPolicy(Qt::NoFocus);
        // 使最大化按钮不可见
        restore_button->setVisible(false);
    }
    else
    {
        // 创建PushButton
        close_button = new QPushButton(this);
        // 设置PushButtond的大小
        close_button->setFixedSize(46,this->height());
        // 设置PushButton的样式
        close_button->setStyleSheet("QPushButton{border-image: url(:/img/button/close_normal.png);}"
                                  "QPushButton:hover{border-image: url(:/img/button/close_hover.png);}"
                                  "QPushButton:pressed{border-image: url(:/img/button/close_pressed.png);}");
        // PushButton无焦点
        close_button->setFocusPolicy(Qt::NoFocus);
    }

    //
    if (isShowAllButton == true)
    {
        QHBoxLayout *layout = new QHBoxLayout();
        QSpacerItem *hSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addWidget(icon,0);
        layout->addWidget(title_text,1);
        layout->addSpacerItem(hSpacer);
        layout->addWidget(menu_button,2);
        layout->addWidget(min_button,3);
        layout->addWidget(max_button,4);
        layout->addWidget(restore_button,4);
        layout->addWidget(close_button,5);
        layout->setSpacing(0);
        layout->setContentsMargins(5,0,0,0);
        this->setLayout(layout);
    }
    else
    {
        QHBoxLayout *layout = new QHBoxLayout();
        QSpacerItem *hSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addWidget(icon,0);
        layout->addWidget(title_text,1);
        layout->addSpacerItem(hSpacer);
        layout->addWidget(close_button,2);
        layout->setSpacing(0);
        layout->setContentsMargins(5,0,0,0);
        this->setLayout(layout);
    }

}

TitleBarWidget::~TitleBarWidget()
{

}

void TitleBarWidget::initConnect()
{
    if (isShowAllButton == true)
    {
        connect(menu_button,    SIGNAL(clicked()), parent_widget, SLOT(showMainMenu())   );
        connect(min_button,     SIGNAL(clicked()), parent_widget, SLOT(showMinimized())  );
        connect(max_button,     SIGNAL(clicked()), this,          SLOT(maximizeWindow()) );
        connect(restore_button, SIGNAL(clicked()), this,          SLOT(restoreWindow())  );
        connect(close_button,   SIGNAL(clicked()), this,          SIGNAL(closeWindow())  );
    }
    else
    {
        connect(close_button,   SIGNAL(clicked()), this,          SIGNAL(closeWindow())  );
    }
}

void TitleBarWidget::maximizeWindow()
{
    if (window_maximize == false)
    {
        parent_widget->showMaximized();
        max_button->setVisible(false);
        restore_button->setVisible(true);
    }
    window_maximize = true;
}

void TitleBarWidget::restoreWindow()
{
    if (window_maximize == true)
    {
        parent_widget->showNormal();
        max_button->setVisible(true);
        restore_button->setVisible(false);
    }
    window_maximize = false;
}

void TitleBarWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        mouse_pressed = true;
    drag_position = event->globalPos() - parent_widget->pos();
}

void TitleBarWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (isShowAllButton == true)
    {
        if(event->button() == Qt::LeftButton)
        {
            if (window_maximize == true)
                restoreWindow();
            else
                maximizeWindow();
        }
    }
}

void TitleBarWidget::mouseReleaseEvent(QMouseEvent *)
{
    mouse_pressed = false;
    parent_widget-> setWindowOpacity(1);

    if (isShowAllButton == true)
    {
        if (parent_widget->y() <= 0)
        {
            maximizeWindow();
        }
    }
}

void TitleBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_pressed == true)
    {
        if (window_maximize == true && isShowAllButton == true)
        {
            restoreWindow();
        }
        else
        {
            QPoint movePosition = event->globalPos();
            parent_widget->move(movePosition - drag_position);
            parent_widget->setWindowOpacity(0.3);
        }
    }
}


