#ifndef MESSAGE_PUSH_H
#define MESSAGE_PUSH_H

#include <QWidget>


namespace Ui {
class message_push;
}

class message_push : public QWidget
{
    Q_OBJECT
public:
    explicit message_push(QWidget *parent = nullptr);
    ~message_push();

signals:

public slots:

private:
    Ui::message_push  *ui;
};


#endif // MESSAGE_PUSH_H
