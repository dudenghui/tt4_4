#include "rolling.h"
#include "mainwindow.h"

rolling::rolling(QWidget *parent)
    : QLabel(parent)
{
    this->resize(500,500);

    QTimer *timer = new QTimer(this);   //声明一个定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));  //连接信号槽，定时器超时触发窗体更新
    timer->start(50);   //启动定时器
}

rolling::~rolling()
{

}


void rolling::paintEvent(QPaintEvent *event)
{

    QColor mainColor(0x4E,0x6D,0x8C,255);  //指针颜色(第四个表示不透明度)


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);//必须先激活才能有这种操作
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    painter.save();
    QFont font=painter.font();
    font.setPixelSize(20);
    font.setFamily("YouYuan");
    painter.setFont(font);
    QPen pen;
    painter.setPen(mainColor);
    painter.drawText(5,5,100,20,Qt::AlignLeft | Qt::AlignTop,"横滚角:");
    painter.drawText(80,5,60,20,Qt::AlignLeft | Qt::AlignTop,QString("%1").arg(angle));
    painter.drawText(130,5,30,20,Qt::AlignLeft | Qt::AlignTop,"度");
    painter.restore();

    painter.translate(0.5*width(),0.6*height());
    painter.save();
    painter.rotate(angle);

    QPixmap pix;
    pix.load(":/image/pitch");
    painter.setBackground(Qt::white);
    painter.scale(0.3,0.3);
    painter.drawPixmap(-0.5*pix.width(),-0.55*pix.height(),pix);
    painter.restore();
    painter.save();

    pen.setWidth(3);
    pen.setColor(mainColor);
    pen.setStyle(Qt::DashDotLine);
    painter.setPen(pen);
    painter.drawLine(-1000,0.02*pix.height(),1000,0.02*pix.height());
}



