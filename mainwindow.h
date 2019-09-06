#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rolling.h"
#include "pitch.h"
#include "dial.h"

#include <QLabel>

#include <QtNetwork>
#include <QTcpServer>
#include <QUdpSocket>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QSplitter>
#include <QtSql>
#include <QFileDialog>

#include <QTime>
#include <QTimer>
#include <QDate>

namespace Ui {
class MainWindow;
}

struct Gra_para
{
    float angleHead;
    float anglePitch;
    float angleRolling;
};
union UByte4//用来对整型数据进行字符拆分
{
    unsigned int zheng;
    unsigned char zifu[4];
};
union Byte4
{
    int zheng;
    char zifu[4];
};

union UByte2
{
    unsigned short zheng;
    unsigned char zifu[2];
};
union Byte2
{
    short zheng;
    char zifu[2];
};

struct ZongDao
{//综导台协议
    char InfoNo;//信息单元序号
    char InfoSign;//信息单元标识 FFH
    UByte2 InfoLength;//信息单元长度 0027H
    UByte2 InfoSignExtend;//扩展信息单元标识 A202H
    UByte2 InfoState;//状态及数据有效标志
    Byte4 Longitude;//经度
    Byte4 Latitude;//纬度
    Byte2 SpeedNorth;//北速
    Byte2 SpeedEast;//东速
    Byte2 SpeedVertical;//升沉速度
    Byte2 Speed;//航速
    Byte2 Depth;//下潜深度
    UByte4 DepthBelow;//艇下深
    UByte4 SeaDepth;//海深
    char SeaState;//海况
    Byte4 AngelHead;//艏向角
    UByte2 Year;//年
    char Month;//月
    char Day;//日
    char Hour;//时
    char Minute;//分
    char Second;//秒
    Byte4 AngleTrack;//航迹角
};

struct Log
{//＄VMWCV,vv.vv,s*CC<CR><LF>
    double Speed;//航速，单位为节（kn）
    char sign;//P表示正，N表示负
};

struct Gyrocompass
{//	＄HEHDT,xxx.xxx,T*hh<CR><LF>
     double AngleHead;//艇向角 精确到0.01°，不足位补0
     char T;//航向标志
};

struct GPS
{
     int type;//数据类型：1=差分修正的位置数据，0=没有差分修正的位置数据
     int qq;//定位计算时使用的卫星数
     int hour,minute;//hhmmss.ss———定位计算的UTC时间：时、分、秒（精确到百分之一秒）
     double second;
     int latitude_du;//ddmm.mmmmmm———当前纬度值：dd度；mm.mmmmmm分
     double latitude_fen;//s———纬度符号：N=北纬，S=南纬
     char latitude_sign;
     int longtitude_du;//dddmm.mmmmmm———当前经度值：ddd度；mm.mmmmmm分
     double longtitude_fen;//s———经度符号：E=东经，W=西经
     double height;//saaaaa.aaa———s=+/-,表示以WGS-84坐标系为基础的海平面高度；米（精确到千分之一米）（三维定位时由定位数据解算，二维定位时则根据用户设定）
     double angleHead;//ttt.tt———航迹向，单位：度（°）
     double speedToGround;//ggg.gg———对地速度，单位：节（kn）
     double speedVertical;//svvv.vv———s=+/-,垂直速度，单位：分米/秒
     //pp.p———PDOP值，位置误差放大系数
     //hh.h———HDOP值，水平误差放大系数
     //vv.v———VDOP值，垂直误差放大系数
     //tt.t———TDOP值，时间误差放大系数
     //vvvv———软件版本号
};
struct DataDisplay
{
    ZongDao rowData;
    float zd_longitude;
    float zd_latitude;
    float zd_anglehead;
    float zd_angle_track;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel  *LabSocketState;

    QUdpSocket  *udpSocket;//用于与连接的客户端通讯的QTcpSocket
    QHostAddress    groupAddress;//组播地址

    QLabel  *LabListen;//状态栏标签  监听状态
    //QLabel  *LabSocketState;//状态栏标签
    QTcpServer *tcpServer; //TCP服务器
    QTcpSocket *tcpSocket;//TCP通讯的Socket

    QString getLocalIP();//获取本机IP地址

    QLabel  *SerialState;

    QSqlDatabase  DB;//数据库连接
    QSqlTableModel  *tabModel;  //数据模型
//    QItemSelectionModel *theSelection; //选择模型

    QTimer *fTimer;//定时器
    QTimer *fTimer1;//


protected:
    void    closeEvent(QCloseEvent *event);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void serialportscan();//串口扫描
    bool serialopen();//串口打开

private slots:
//自定义槽函数
    void onSocketStateChange(QAbstractSocket::SocketState socketState);

    /********************UDP组播****************************/
    void onSocketReadyRead_udp();//读取socket传入的数据
    void on_pushButton_join_multicast_clicked();
    void on_pushButton_quit_multicast_clicked();
    void on_pushButton_clear_udp_clicked();
    void on_pushButton_send_multicast_clicked();

    /********************TCP服务器****************************/
    void onNewConnection();//QTcpServer的newConnection()信号
    void onClientConnected(); //Client Socket connected
    void onClientDisconnected();//Client Socket disconnected
    void onSocketReadyRead_TCP();//读取socket传入的数据
    void on_pushButton_start_watch_clicked();
    void on_pushButton_stop_watch_clicked();
    void on_pushButton_clear_tcp_clicked();

    void on_pushButton_send_message_clicked();//一键发送

    /********************串口通信****************************/
    void SerialRecvMsgEvent();//串口接收事件
    void on_pushButton_Serial_Open_clicked();
    void on_pushButton_Serial_Close_clicked();
    void on_pushButton_Serial_Clear_clicked();
    void on_pushButton_Serial_Refresh_clicked();
    void on_pushButton_Serial_Send_clicked();

    void on_pushButton_connect_clicked();
    void on_pushButton_disconnect_clicked();

    void on_pushButton_DB_open_clicked();
    void on_pushButton_DB_close_clicked();
    void on_comboBox_DB_table_currentTextChanged(const QString &arg1);
    void on_spinBox_DB_time_valueChanged(int arg1);
    void on_pushButton_DB_add_clicked();
    void on_pushButton_DB_delete_clicked();
    void on_pushButton_DB_insert_clicked();
    void on_pushButton_DB_save_clicked();
    void on_timer_timeout();
    void on_timer_timeout1();
    void on_pushButton_DB_Counter_clicked();

    void on_LineEdit_ins_anglehead_textChanged(const QString &arg1);

    void on_LineEdit_ins_anglerolling_textChanged(const QString &arg1);

    void on_LineEdit_ins_anglepitch_textChanged(const QString &arg1);

    void on_pushButton_refresh_clicked();

private:
    Ui::MainWindow *ui;

    QSerialPort *mserial;
    QByteArray buf;

    QByteArray zd_buf;
    ZongDao RecMessage;//保存综导台接受的信息

    DataDisplay datadisp;

};


#endif // MAINWINDOW_H
