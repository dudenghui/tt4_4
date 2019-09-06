#ifndef DIAL_H
#define DIAL_H

#include <QLabel>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QList>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QToolTip>


class dial: public QLabel
{
    //Q_OBJECT
public:
    float angle;
public:
    dial(QWidget *parent = 0);

    ~dial();

protected:
    void paintEvent(QPaintEvent *event);


private:

};


#endif // DIAL_H
