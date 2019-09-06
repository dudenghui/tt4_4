#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::getLocalIP()
{//获取本地主机名
    QString hostName=QHostInfo::localHostName();//本地主机名
    QHostInfo   hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";

    QList<QHostAddress> addList=hostInfo.addresses();//

    if (!addList.isEmpty())
    for (int i=0;i<addList.count();i++)
    {
        QHostAddress aHost=addList.at(i);
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            localIP=aHost.toString();
            break;
        }
    }
    return localIP;
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)       //响应套接字状态改变，在状态栏显示套接字的状态
{//响应状态改变 在状态栏予以显示
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        LabSocketState->setText("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        LabSocketState->setText("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        LabSocketState->setText("scoket状态：ConnectingState");
        break;

    case QAbstractSocket::ConnectedState:
        LabSocketState->setText("scoket状态：ConnectedState");
        break;

    case QAbstractSocket::BoundState:
        LabSocketState->setText("scoket状态：BoundState");
        break;

    case QAbstractSocket::ClosingState:
        LabSocketState->setText("scoket状态：ClosingState");
        break;

    case QAbstractSocket::ListeningState:
        LabSocketState->setText("scoket状态：ListeningState");
    }
}

void MainWindow::onSocketReadyRead_udp()    //UDP组播准备读取响应函数
{//读取数据报
    ui->plainTextEdit_multicast->appendPlainText("接到数据");

    while(udpSocket->hasPendingDatagrams())
    {
        zd_buf.resize(udpSocket->pendingDatagramSize());
        QHostAddress    peerAddr;
        quint16 peerPort;
        udpSocket->readDatagram(zd_buf.data(),zd_buf.size(),&peerAddr,&peerPort);   //获取消息的IP地址以及端口，并从套接字中
        QString peer="[From "+peerAddr.toString()+":"+QString::number(peerPort)+"] ";
        ui->plainTextEdit_multicast->appendPlainText(peer);

        if(zd_buf[1]==0xff&&(zd_buf[3]<<8|zd_buf[2])==0x0027)//数据验证
        {
            ui->plainTextEdit_multicast->appendPlainText("数据验收成功");
            RecMessage.InfoNo=zd_buf[0];
            RecMessage.InfoSign=zd_buf[1];
            RecMessage.InfoLength.zifu[0]=zd_buf[2];
            RecMessage.InfoLength.zifu[1]=zd_buf[3];
            RecMessage.InfoSignExtend.zifu[0]=zd_buf[4];
            RecMessage.InfoSignExtend.zifu[1]=zd_buf[5];
            RecMessage.InfoState.zifu[0]=zd_buf[6];
            RecMessage.InfoState.zifu[1]=zd_buf[7];
            RecMessage.Longitude.zifu[0]=zd_buf[8];
            RecMessage.Longitude.zifu[1]=zd_buf[9];
            RecMessage.Longitude.zifu[2]=zd_buf[10];
            RecMessage.Longitude.zifu[3]=zd_buf[11];
            RecMessage.Latitude.zifu[0]=zd_buf[12];
            RecMessage.Latitude.zifu[1]=zd_buf[13];
            RecMessage.Latitude.zifu[2]=zd_buf[14];
            RecMessage.Latitude.zifu[3]=zd_buf[15];
            RecMessage.SpeedNorth.zifu[0]=zd_buf[16];
            RecMessage.SpeedNorth.zifu[1]=zd_buf[17];
            RecMessage.SpeedEast.zifu[0]=zd_buf[18];
            RecMessage.SpeedEast.zifu[1]=zd_buf[19];
            RecMessage.SpeedVertical.zifu[0]=zd_buf[20];
            RecMessage.SpeedVertical.zifu[1]=zd_buf[21];
            RecMessage.Speed.zifu[0]=zd_buf[22];
            RecMessage.Speed.zifu[1]=zd_buf[23];
            RecMessage.Depth.zifu[0]=zd_buf[24];
            RecMessage.Depth.zifu[1]=zd_buf[25];
            RecMessage.DepthBelow.zifu[0]=zd_buf[26];
            RecMessage.DepthBelow.zifu[1]=zd_buf[27];
            RecMessage.DepthBelow.zifu[2]=zd_buf[28];
            RecMessage.DepthBelow.zifu[3]=zd_buf[29];
            RecMessage.SeaDepth.zifu[0]=zd_buf[30];
            RecMessage.SeaDepth.zifu[1]=zd_buf[31];
            RecMessage.SeaDepth.zifu[2]=zd_buf[32];
            RecMessage.SeaDepth.zifu[3]=zd_buf[33];
            RecMessage.SeaState=(char)zd_buf[34];
            RecMessage.AngelHead.zifu[0]=zd_buf[35];
            RecMessage.AngelHead.zifu[1]=zd_buf[36];
            RecMessage.AngelHead.zifu[2]=zd_buf[37];
            RecMessage.AngelHead.zifu[3]=zd_buf[38];
            RecMessage.Year.zifu[0]=zd_buf[39];
            RecMessage.Year.zifu[1]=zd_buf[40];
            RecMessage.Month=zd_buf[41];
            RecMessage.Day=zd_buf[42];
            RecMessage.Hour=zd_buf[43];
            RecMessage.Minute=zd_buf[44];
            RecMessage.Second=zd_buf[45];
            RecMessage.AngleTrack.zifu[0]=zd_buf[46];
            RecMessage.AngleTrack.zifu[1]=zd_buf[47];
            RecMessage.AngleTrack.zifu[2]=zd_buf[48];
            RecMessage.AngleTrack.zifu[3]=zd_buf[49];


            datadisp.rowData=RecMessage;
            datadisp.zd_longitude=float(datadisp.rowData.Longitude.zheng)/pow(2,30)*90;
            datadisp.zd_latitude=float(datadisp.rowData.Latitude.zheng)/pow(2,30)*90;
            datadisp.zd_anglehead=float(datadisp.rowData.AngelHead.zheng)/pow(2,31)*360;
            datadisp.zd_angle_track=float(datadisp.rowData.AngleTrack.zheng)/pow(2,31)*360;

            /**************************************显示数据****************************************************/
            ui->plainTextEdit_tcp->appendPlainText(QString("%1").arg(QString(zd_buf),0,16));//文本框

            ui->lineEdit_zd_Longitude->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_longitude*3600)/3600)
                          .arg(int(datadisp.zd_longitude*3600)%3600/60).arg(int(datadisp.zd_longitude*3600)%60));
            ui->lineEdit_zd_Latitude->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_latitude*3600)/3600)
                          .arg(int(datadisp.zd_latitude*3600)%3600/60).arg(int(datadisp.zd_latitude*3600)%60));
            ui->lineEdit_zd_NorthSpeed->setText(QString("%1kn").arg(double(RecMessage.SpeedNorth.zheng)/100));
            ui->lineEdit_zd_EastSpeed->setText(QString("%1kn").arg(double(RecMessage.SpeedEast.zheng)/100));
            ui->lineEdit_zd_UpDownSpeed->setText(QString("%1kn").arg(double(RecMessage.SpeedVertical.zheng)/100));
            ui->lineEdit_zd_ForwardSpeed->setText(QString("%1kn").arg(double(RecMessage.Speed.zheng)/100));
            ui->lineEdit_zd_Depth->setText(QString("%1m").arg(double(RecMessage.Depth.zheng)/10));
            ui->lineEdit_zd_DepthBelow->setText(QString("%1m").arg(double(RecMessage.DepthBelow.zheng)/100));
            ui->lineEdit_zd_SeaDepth->setText(QString("%1m").arg(double(RecMessage.SeaDepth.zheng)/100));
            ui->lineEdit_SeaSituation->setText(QString("%1级").arg(RecMessage.SeaState));
            ui->lineEdit_zd_AngleTracks->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_angle_track*3600)/3600)
                          .arg(int(datadisp.zd_angle_track*3600)%3600/60).arg(int(datadisp.zd_angle_track*3600)%60));
            ui->lineEdit_zd_AngleBow->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_anglehead*3600)/3600)
                          .arg(int(datadisp.zd_anglehead*3600)%3600/60).arg(int(datadisp.zd_anglehead*3600)%60));
            ui->LineEdit_zd_date->setText(QString("%1年%2月%3日").arg(RecMessage.Year.zheng)
                                          .arg((int)RecMessage.Month).arg((int)RecMessage.Day));
            ui->lineEdit_zd_time->setText(QString("%1时%2分%3秒").arg((int)RecMessage.Hour)
                                          .arg((int)RecMessage.Minute).arg((int)RecMessage.Second));
        }

    }

}

void MainWindow::on_pushButton_join_multicast_clicked()  //加入组播
{//加入组播
    QString     IP=ui->comboBox_multicast->currentText();
    groupAddress=QHostAddress(IP);//多播组地址
    quint16     groupPort=ui->spinBox_multicast->value();//端口

    if (udpSocket->bind(QHostAddress::AnyIPv4, groupPort, QUdpSocket::ShareAddress))//先绑定端口
    {
        udpSocket->joinMulticastGroup(groupAddress); //加入多播组
        ui->plainTextEdit_multicast->appendPlainText("**加入组播成功");
        ui->plainTextEdit_multicast->appendPlainText("**组播地址IP："+IP);
        ui->plainTextEdit_multicast->appendPlainText("**绑定端口："+QString::number(groupPort));
        ui->pushButton_join_multicast->setEnabled(false);
        ui->pushButton_quit_multicast->setEnabled(true);
        ui->pushButton_send_multicast->setEnabled(true);
        ui->comboBox_multicast->setEnabled(false);
    }
    else
        ui->plainTextEdit_multicast->appendPlainText("**绑定端口失败");
}

void MainWindow::on_pushButton_quit_multicast_clicked()  //退出组播
{//退出组播
    udpSocket->leaveMulticastGroup(groupAddress);//退出组播
    udpSocket->abort(); //解除绑定
    ui->pushButton_join_multicast->setEnabled(true);
    ui->pushButton_quit_multicast->setEnabled(false);
    ui->pushButton_send_multicast->setEnabled(false);
    ui->comboBox_multicast->setEnabled(true);
    ui->plainTextEdit_multicast->appendPlainText("**已退出组播,解除端口绑定");

}

void MainWindow::on_pushButton_clear_udp_clicked()
{
    ui->plainTextEdit_multicast->clear();
}

void MainWindow::on_pushButton_send_multicast_clicked()  //发送组播消息
{//发送组播消息
    quint16     groupPort=ui->spinBox_multicast->value();
    QString  msg=ui->lineEditmulticast->text();
    QByteArray  datagram=msg.toUtf8();

    udpSocket->writeDatagram(datagram,groupAddress,groupPort);
//    udpSocket->writeDatagram(datagram.data(),datagram.size(),
//                     groupAddress,groupPort);
    ui->plainTextEdit_multicast->appendPlainText("[multicst] "+msg);
//    ui->editMsg->clear();
    ui->lineEditmulticast->setFocus();

}



/*************************************以下为TCP通信部分***********************************************/
void MainWindow::onNewConnection()
{
//    ui->plainTextEdit->appendPlainText("有新连接");
    tcpSocket = tcpServer->nextPendingConnection(); //创建socket

    connect(tcpSocket, SIGNAL(connected()),this, SLOT(onClientConnected()));
    onClientConnected();//

    connect(tcpSocket, SIGNAL(disconnected()),this, SLOT(onClientDisconnected()));

    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(tcpSocket->state());

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead_TCP()));
}

void MainWindow::onClientConnected()
{//客户端接入时
    ui->plainTextEdit_tcp->appendPlainText("**client socket connected");
    ui->plainTextEdit_tcp->appendPlainText("**peer address:"+tcpSocket->peerAddress().toString());
    ui->plainTextEdit_tcp->appendPlainText("**peer port:"+QString::number(tcpSocket->peerPort()));

    fTimer->start();
}

void MainWindow::onClientDisconnected()
{//客户端断开连接时
    ui->plainTextEdit_tcp->appendPlainText("**client socket disconnected");
    tcpSocket->deleteLater();
    //    deleteLater();//QObject::deleteLater();
    fTimer->stop();
}

void MainWindow::onSocketReadyRead_TCP()
{//读取缓冲区行文本
    while(tcpSocket->canReadLine())
    {
        zd_buf=tcpSocket->readAll();
        if(zd_buf[1]==0xff&&(zd_buf[3]<<8|zd_buf[2])==0x0027)//数据验证
        {
            qDebug()<<"数据验收成功";
            RecMessage.InfoNo=zd_buf[0];
            RecMessage.InfoSign=zd_buf[1];
            RecMessage.InfoLength.zifu[0]=zd_buf[2];
            RecMessage.InfoLength.zifu[1]=zd_buf[3];
            RecMessage.InfoSignExtend.zifu[0]=zd_buf[4];
            RecMessage.InfoSignExtend.zifu[1]=zd_buf[5];
            RecMessage.InfoState.zifu[0]=zd_buf[6];
            RecMessage.InfoState.zifu[1]=zd_buf[7];
            RecMessage.Longitude.zifu[0]=zd_buf[8];
            RecMessage.Longitude.zifu[1]=zd_buf[9];
            RecMessage.Longitude.zifu[2]=zd_buf[10];
            RecMessage.Longitude.zifu[3]=zd_buf[11];
            RecMessage.Latitude.zifu[0]=zd_buf[12];
            RecMessage.Latitude.zifu[1]=zd_buf[13];
            RecMessage.Latitude.zifu[2]=zd_buf[14];
            RecMessage.Latitude.zifu[3]=zd_buf[15];
            RecMessage.SpeedNorth.zifu[0]=zd_buf[16];
            RecMessage.SpeedNorth.zifu[1]=zd_buf[17];
            RecMessage.SpeedEast.zifu[0]=zd_buf[18];
            RecMessage.SpeedEast.zifu[1]=zd_buf[19];
            RecMessage.SpeedVertical.zifu[0]=zd_buf[20];
            RecMessage.SpeedVertical.zifu[1]=zd_buf[21];
            RecMessage.Speed.zifu[0]=zd_buf[22];
            RecMessage.Speed.zifu[1]=zd_buf[23];
            RecMessage.Depth.zifu[0]=zd_buf[24];
            RecMessage.Depth.zifu[1]=zd_buf[25];
            RecMessage.DepthBelow.zifu[0]=zd_buf[26];
            RecMessage.DepthBelow.zifu[1]=zd_buf[27];
            RecMessage.DepthBelow.zifu[2]=zd_buf[28];
            RecMessage.DepthBelow.zifu[3]=zd_buf[29];
            RecMessage.SeaDepth.zifu[0]=zd_buf[30];
            RecMessage.SeaDepth.zifu[1]=zd_buf[31];
            RecMessage.SeaDepth.zifu[2]=zd_buf[32];
            RecMessage.SeaDepth.zifu[3]=zd_buf[33];
            RecMessage.SeaState=(char)zd_buf[34];
            RecMessage.AngelHead.zifu[0]=zd_buf[35];
            RecMessage.AngelHead.zifu[1]=zd_buf[36];
            RecMessage.AngelHead.zifu[2]=zd_buf[37];
            RecMessage.AngelHead.zifu[3]=zd_buf[38];
            RecMessage.Year.zifu[0]=zd_buf[39];
            RecMessage.Year.zifu[1]=zd_buf[40];
            RecMessage.Month=zd_buf[41];
            RecMessage.Day=zd_buf[42];
            RecMessage.Hour=zd_buf[43];
            RecMessage.Minute=zd_buf[44];
            RecMessage.Second=zd_buf[45];
            RecMessage.AngleTrack.zifu[0]=zd_buf[46];
            RecMessage.AngleTrack.zifu[1]=zd_buf[47];
            RecMessage.AngleTrack.zifu[2]=zd_buf[48];
            RecMessage.AngleTrack.zifu[3]=zd_buf[49];
        }

        datadisp.rowData=RecMessage;
        datadisp.zd_longitude=float(datadisp.rowData.Longitude.zheng)/pow(2,30)*90;
        datadisp.zd_latitude=float(datadisp.rowData.Latitude.zheng)/pow(2,30)*90;
        datadisp.zd_anglehead=float(datadisp.rowData.AngelHead.zheng)/pow(2,31)*360;
        datadisp.zd_angle_track=float(datadisp.rowData.AngleTrack.zheng)/pow(2,31)*360;

        ui->plainTextEdit_tcp->appendPlainText(QString("%1").arg(QString(zd_buf),0,16));//文本框

        ui->lineEdit_zd_Longitude->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_longitude*3600)/3600)
                      .arg(int(datadisp.zd_longitude*3600)%3600/60).arg(int(datadisp.zd_longitude*3600)%60));
        ui->lineEdit_zd_Latitude->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_latitude*3600)/3600)
                      .arg(int(datadisp.zd_latitude*3600)%3600/60).arg(int(datadisp.zd_latitude*3600)%60));
        ui->lineEdit_zd_NorthSpeed->setText(QString("%1kn").arg(double(RecMessage.SpeedNorth.zheng)/100));
        ui->lineEdit_zd_EastSpeed->setText(QString("%1kn").arg(double(RecMessage.SpeedEast.zheng)/100));
        ui->lineEdit_zd_UpDownSpeed->setText(QString("%1kn").arg(double(RecMessage.SpeedVertical.zheng)/100));
        ui->lineEdit_zd_ForwardSpeed->setText(QString("%1kn").arg(double(RecMessage.Speed.zheng)/100));
        ui->lineEdit_zd_Depth->setText(QString("%1m").arg(double(RecMessage.Depth.zheng)/10));
        ui->lineEdit_zd_DepthBelow->setText(QString("%1m").arg(double(RecMessage.DepthBelow.zheng)/100));
        ui->lineEdit_zd_SeaDepth->setText(QString("%1m").arg(double(RecMessage.SeaDepth.zheng)/100));
        ui->lineEdit_SeaSituation->setText(QString("%1级").arg(RecMessage.SeaState));
        ui->lineEdit_zd_AngleTracks->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_angle_track*3600)/3600)
                      .arg(int(datadisp.zd_angle_track*3600)%3600/60).arg(int(datadisp.zd_angle_track*3600)%60));
        ui->lineEdit_zd_AngleBow->setText(QString("%1度 %3分 %5秒").arg(int(datadisp.zd_anglehead*3600)/3600)
                      .arg(int(datadisp.zd_anglehead*3600)%3600/60).arg(int(datadisp.zd_anglehead*3600)%60));
        ui->LineEdit_zd_date->setText(QString("%1年%2月%3日").arg(RecMessage.Year.zheng)
                                      .arg((int)RecMessage.Month).arg((int)RecMessage.Day));
        ui->lineEdit_zd_time->setText(QString("%1时%2分%3秒").arg((int)RecMessage.Hour)
                                      .arg((int)RecMessage.Minute).arg((int)RecMessage.Second));
        //lines.append(clientConnection->readLine());
    }


}

void MainWindow::on_pushButton_start_watch_clicked()
{//开始监听
    QString     IP=ui->comboBox_tcp->currentText();//IP地址
    quint16     port=ui->spinBox_tcp->value();//端口
    QHostAddress    addr(IP);
    tcpServer->listen(addr,port);//
//    tcpServer->listen(QHostAddress::LocalHost,port);// Equivalent to QHostAddress("127.0.0.1").
    ui->plainTextEdit_tcp->appendPlainText("**开始监听...");
    ui->plainTextEdit_tcp->appendPlainText("**服务器地址："+tcpServer->serverAddress().toString());
    ui->plainTextEdit_tcp->appendPlainText("**服务器端口："+QString::number(tcpServer->serverPort()));

    ui->pushButton_start_watch->setEnabled(false);
    ui->pushButton_stop_watch->setEnabled(true);

    LabListen->setText("监听状态：正在监听");

}

void MainWindow::on_pushButton_stop_watch_clicked()
{//停止监听
    if (tcpServer->isListening()) //tcpServer正在监听
    {
        tcpServer->close();//停止监听
        ui->pushButton_start_watch->setEnabled(true);
        ui->pushButton_stop_watch->setEnabled(false);
        LabListen->setText("监听状态：已停止监听");
    }

}

void MainWindow::on_pushButton_clear_tcp_clicked()
{
    ui->plainTextEdit_tcp->clear();
}

void MainWindow::on_pushButton_send_message_clicked()
{//发送一行字符串，以换行符结束
    QString  msg=ui->lineEdit_tcp->text();
    ui->plainTextEdit_tcp->appendPlainText("[out] "+msg);
    ui->lineEdit_tcp->setFocus();

    QByteArray  str=msg.toUtf8();
    str.append('\n');//添加一个换行符
    tcpSocket->write(str);

}
