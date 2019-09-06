#include "message_push.h"
#include "ui_message_push.h"

message_push::message_push(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message_push)
{
    ui->setupUi(this);
}

message_push::~message_push()
{
    delete ui;
}
