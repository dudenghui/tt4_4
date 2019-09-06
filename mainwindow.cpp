#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

Gra_para para;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file(":/qss/qss1.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);//设置qss样式表
    file.close();

    QString localIP=getLocalIP();//本地主机名
    QString name="导航信息显控终端";
    this->setWindowTitle(name+"----本机IP："+localIP);
    this->setWindowIcon(QIcon(":/icon/duo.png"));
    QSize icon_size(64,64);
    this->setIconSize(icon_size);
/****************************************组播通信部分******************************************/
    LabSocketState=new QLabel("UdpSocket状态：");//
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    udpSocket=new QUdpSocket(this);//用于与连接的客户端通讯的QTcpSocketMulticast路由层次，
                                   //1表示只在同一局域网内组播TTL: 生存时间，每跨1个路由会减1，
                                   //多播无法跨过大多数路由所以为1默认值是1，表示数据包只能在本地的子网中传送。
    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,1);

    connect(udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(udpSocket->state());
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead_udp()));

    ui->pushButton_join_multicast->setEnabled(true);
    ui->pushButton_quit_multicast->setEnabled(false);
    ui->pushButton_send_multicast->setEnabled(false);
/****************************************组播通信部分******************************************/

/****************************************tcp通信部分******************************************/
    LabListen=new QLabel("监听状态: 非监听状态");
    LabListen->setMinimumWidth(150);
    ui->statusBar->addWidget(LabListen);

    LabSocketState=new QLabel("TcpSocket状态：未连接");//
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    ui->comboBox_tcp->addItem(localIP);
    QString localIP1="127.1.1.1";
    ui->comboBox_tcp->addItem(localIP1);

    tcpServer=new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
/****************************************tcp通信部分******************************************/

/****************************************串口通信部分******************************************/
    serialportscan();//扫描串口
    ui->pushButton_Serial_Open->setEnabled(false);
    ui->pushButton_Serial_Close->setEnabled(false);
    ui->pushButton_Serial_Send->setEnabled(false);
    SerialState=new QLabel("串口连接状态");
    SerialState->setMinimumWidth(250);
    ui->statusBar->addWidget(SerialState);
/****************************************串口通信部分******************************************/

/****************************************数据库部分******************************************/
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableView->setAlternatingRowColors(true);

    fTimer1=new QTimer(this);//定时器设置
    fTimer1->stop();
    fTimer1->setInterval(50);
    connect(fTimer1,SIGNAL(timeout()),this,SLOT(on_timer_timeout1()));

    on_pushButton_DB_open_clicked();//直接打开数据库

//    ui->pushButton_DB_add->setEnabled(false);
//    ui->pushButton_DB_close->setEnabled(false);
//    ui->pushButton_DB_delete->setEnabled(false);
//    ui->pushButton_DB_insert->setEnabled(false);
//    ui->pushButton_DB_save->setEnabled(false);
/****************************************数据库部分******************************************/

    fTimer=new QTimer(this);//定时器设置
    fTimer->stop();
    fTimer->setInterval(2000);
    ui->spinBox_DB_time->setValue(2000);
    connect(fTimer,SIGNAL(timeout()),this,SLOT(on_timer_timeout()));

}

MainWindow::~MainWindow()
{
    delete ui;
    udpSocket->abort();
    delete udpSocket;
}

void MainWindow::on_pushButton_connect_clicked()
{//一键连接
    on_pushButton_start_watch_clicked();

    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(true);
}

void MainWindow::on_pushButton_disconnect_clicked()
{//一键断开
    on_pushButton_stop_watch_clicked();

    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{//关闭窗口时停止监听
    if (tcpServer->isListening())
        tcpServer->close();//停止网络监听
    event->accept();
}






void MainWindow::on_LineEdit_ins_anglehead_textChanged(const QString &arg1)
{

}

void MainWindow::on_LineEdit_ins_anglerolling_textChanged(const QString &arg1)
{

}

void MainWindow::on_LineEdit_ins_anglepitch_textChanged(const QString &arg1)
{

}

void MainWindow::on_pushButton_refresh_clicked()
{
    if(ui->pushButton_refresh->text()=="刷新")
    {
        ui->pushButton_refresh->setText("停止");
        fTimer1->start();
    }
    else if(ui->pushButton_refresh->text()=="停止")
    {
        ui->pushButton_refresh->setText("从文本框获取");
        fTimer1->stop();
        ui->label_6->angle=0;
        ui->label_pitch->angle=0;
        ui->label_rolling->angle=0;
        ui->LineEdit_ins_anglepitch->setText(QString("%1").arg(ui->label_pitch->angle));
        ui->LineEdit_ins_anglerolling->setText(QString("%1").arg(ui->label_rolling->angle));
        ui->LineEdit_ins_anglehead->setText(QString("%1").arg(ui->label_6->angle));
    }
    else if(ui->pushButton_refresh->text()=="从文本框获取")
    {
        ui->pushButton_refresh->setText("刷新");
        ui->label_6->angle=ui->LineEdit_ins_anglehead->text().toFloat();
        ui->label_pitch->angle=ui->LineEdit_ins_anglepitch->text().toFloat();
        ui->label_rolling->angle=ui->LineEdit_ins_anglerolling->text().toFloat();
    }
}
void MainWindow::on_timer_timeout1()
{
    ui->label_6->angle+=1;
    ui->label_pitch->angle+=0.25;
    ui->label_rolling->angle+=0.25;
    if(ui->label_6->angle>360)
    {
        ui->label_6->angle=0;
    }
    if(ui->label_pitch->angle>10)
    {
        ui->label_pitch->angle=-10;
    }
    if(ui->label_rolling->angle>10)
    {
        ui->label_rolling->angle=-10;
    }
    ui->LineEdit_ins_anglepitch->setText(QString("%1").arg(ui->label_pitch->angle));
    ui->LineEdit_ins_anglerolling->setText(QString("%1").arg(ui->label_rolling->angle));
    ui->LineEdit_ins_anglehead->setText(QString("%1").arg(ui->label_6->angle));
}
