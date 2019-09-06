#ifndef PITCH_H
#define PITCH_H

#include <QLabel>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QList>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QToolTip>


class pitch: public QLabel
{
    //Q_OBJECT
public:
    float angle;
public:
    pitch(QWidget *parent = 0);

    ~pitch();

protected:
    void paintEvent(QPaintEvent *event);


private:

};

#endif // PITCH_H
