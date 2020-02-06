#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QPainter>
#include <QDebug>

AboutWindow::AboutWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AboutWindow)
{
  parentWidget = parent;

  ui->setupUi(this);

  initUI();
}

AboutWindow::~AboutWindow()
{
  delete ui;
}

void AboutWindow::closeWindow()
{
  this->close();
  this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void AboutWindow::paintEvent(QPaintEvent *)
{
  QPainterPath path;
  path.setFillRule(Qt::WindingFill);
  path.addRect(0, 0, this->width(), this->height());

  QPainter painter(this);
  painter.fillPath(path, QBrush(Qt::white));

  path.setFillRule(Qt::WindingFill);
  path.addRect(0, 0, this->width()-1, this->height()-1);

  painter.setPen("#0063B1");
  painter.drawPath(path);
}

void AboutWindow::initUI()
{
    QString str;

    this->setMinimumSize(DEFAULT_ABOUT_WINDOW_WIDTH, DEFAULT_ABOUT_WINDOW_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
//    this->setWindowIcon(QIcon(":/img/etc/logo.png"));
    this->setWindowIcon(QIcon(":/img/etc/icon.png"));
    this->setWindowTitle(tr("关于"));
    this->setWindowOpacity(1);  //窗体透明度设置

    titleBarWidget = new TitleBarWidget(this, ABOUT_WINDOW_TITLE, 30, false);
    titleBarWidget->initConnect();
    connect(titleBarWidget, SIGNAL(closeWindow()), this, SLOT(closeWindow()));

    gifLabel = new QLabel(this);
    gifLabel->setFixedSize(this->width(), 300);
    gifLabel->move(0, 30);
    QMovie *movie=new QMovie(":/img/etc/start.gif");
    gifLabel->setMovie(movie);
    movie->start();

    titleText = new QLabel(this);
    titleText->setFixedSize(gifLabel->width(), gifLabel->height());
    titleText->move(0, titleBarWidget->height());
    str.sprintf("<p align=\"center\"><span style=\" font-family:'微软雅黑'; font-size:36pt; color:#ffffff;\">%s</span><span style=\" font-family:'微软雅黑'; font-size:24pt; color:#ffffff;\"> %s</span></p>",MAIN_WINDOW_TITLE,UDSO_VERSION);
//    str += "<p align=\"center\"><span style=\"font-family:'微软雅黑'; font-size:12pt; color:#ffffff;\">The open source, free, cross platform virtual oscilloscope.</span></p></body></html>";
    titleText->setText(str);

    text = new QLabel(this);
    text->setFixedSize(this->width()-20, this->height() - titleBarWidget->height() - gifLabel->height()-30);
    text->move(10, titleBarWidget->height() + gifLabel->height()+5);
    text->setStyleSheet("QLabel{color:black;}");
    text->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    str.sprintf("作者：%s \n更新日期：%s \nEmail：985424805@qq.com", USDO_AUTHOR,  UDSO_UPDATE);
    text->setText(str);

    text1 = new QLabel(this);
    text1->setFixedSize(this->width()-20, 20);
    text1->move(10, titleBarWidget->height() + gifLabel->height() + text->height());
    text1->setStyleSheet("QLabel{color:green;}");
    text1->setFont(QFont("微软雅黑", 10, QFont::Bold, false));
    text1->setAlignment(Qt::AlignVCenter);

    subtitleString = "    μDSO是自由软件；您可以按照自由软件基金会所发表的 GNU GPL 协议自由发放和/或修改它；采用协议的第 3 版，或者(根据您的选择)任何更高版本。";
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateSubtitle()));
    mTimer->start(300);
}

void AboutWindow::updateSubtitle()
{
  static int nPos = 0;

  if (nPos > subtitleString.length())
  {
    nPos = 0;
  }
  text1->setText(subtitleString.mid(nPos));
  nPos++;
}
