#ifndef ROLLING_H
#define ROLLING_H

#include <QLabel>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QList>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QToolTip>

class rolling: public QLabel
{
    //Q_OBJECT
public:
    float angle;
public:
    rolling(QWidget *parent = 0);

    ~rolling();

protected:
    void paintEvent(QPaintEvent *event);


private:

};

#endif // ROLLING_H
