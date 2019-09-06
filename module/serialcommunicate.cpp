#include "mainwindow.h"
#include "ui_mainwindow.h"

/*************************************以下为串口通信部分通信部分***********************************************/

void MainWindow::serialportscan()//@ 扫描存在串口端口
{
  mserial = new QSerialPort();
  if(mserial!=NULL)
  {
    qDebug()<<tr("扫描存在的串口");
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())//静态函数可以不创建对象直接调用，availablePorts()返回一列系统中可得的串行端口
        //QSerialPortInfo类提供一列现存的串行端口信息，foreach起到对每个端口进行遍历的作用
    {
      QSerialPort  ser;
      ser.setPort(info);
      /* 判断端口是否能打开 */
      if(ser.open(QIODevice::ReadWrite))
      {
          ui->comboBox_SerialPort->addItem(ser.portName());
          qDebug()<<tr("Port:")+ser.portName()+tr("\r\n");
          ser.close();

          ui->pushButton_Serial_Open->setEnabled(true);
          ui->pushButton_Serial_Close->setEnabled(false);
          ui->pushButton_Serial_Send->setEnabled(false);
      }
    }
  }
}

bool MainWindow::serialopen()
{//@打开串口
  /*******************************************
   * 设置串口参数
   * 波特率     ：115200
   * 数据位     ：8位
   * 奇偶校验位  ：无
   * 停止位     ：1位
   * 流控制     ：无
   *******************************************/
  mserial->setPortName(ui->comboBox_SerialPort->currentText());  /* 设置串口名称 */
  mserial->setBaudRate(QSerialPort::Baud4800);      /* 设置波特率 */
  mserial->setDataBits(QSerialPort::Data8);           /* 设置数据位数 */
  mserial->setParity(QSerialPort::NoParity);          /* 设置奇偶校验 */
  mserial->setStopBits(QSerialPort::OneStop);         /* 设置停止位*/
  mserial->setFlowControl(QSerialPort::NoFlowControl);/* 设置流控制 */
  mserial->setReadBufferSize(1024);

  if(mserial->open(QIODevice::ReadWrite)==true)/* 打开串口 */
  {
    QObject::connect(mserial, &QSerialPort::readyRead, this, SerialRecvMsgEvent);
    return true;
  }
  else
  {
    return false;
  }
}

//得到指定序号的逗号位置
int GetComma(int num,QByteArray str)
{
    int i,j=0;
    int len=str.count();
    for(i=0;i<len;i++)
    {
        if(str[i]==',')
        {
             j++;
        }
        if(j==num)
            return i+1;
    }
    return 0;
}


void MainWindow::SerialRecvMsgEvent()  //接受数据
{
    Log log;//计程仪
    Gyrocompass gyrocompass;//陀螺罗经
    GPS gps;//GPS

    if(mserial->bytesAvailable() >= 0)
    {
        int index[20];

        buf = mserial->readAll();
        if(buf.contains("$VMWCV"))//代表是复合式计程仪的数据     这里注意是$还是＄
        {//＄VMWCV,vv.vv,s*CC<CR><LF>
            ui->plainTextEdit_Serial_Reciive->appendPlainText("接受来自复合式计程仪的数据");
            for(int i=0;i<2;i++)
            {
                 index[i]=GetComma((i+1),buf);
            }
            log.Speed=buf.mid(index[0],index[1]-index[0]-1).toDouble();
            log.sign=buf[GetComma(2,buf)];
            if(log.sign=='P')
            {
                log.Speed=log.Speed;
                qDebug()<<"正值";
            }
            else if(log.sign=='N')
            {
                log.Speed=(-1)*log.Speed;
                qDebug()<<"负值";
            }
            ui->plainTextEdit_Serial_Reciive->appendPlainText(QString("%1 kn").arg(log.Speed));
            ui->lineEdit_lichengyi_hangsu->setText(QString("%1 kn").arg(log.Speed));
        }
        else if(buf.contains("$HEHDT"))
        {//＄HEHDT,xxx.xxx,T*hh<CR><LF>
            ui->plainTextEdit_Serial_Reciive->appendPlainText("接受来自陀螺罗经的数据");
            for(int i=0;i<2;i++)
            {
                 index[i]=GetComma((i+1),buf);
            }
            gyrocompass.AngleHead=buf.mid(index[0],index[1]-index[0]-1).toDouble();
            gyrocompass.T=buf[GetComma(2,buf)];

            ui->lineEdit_Compass->setText(QString("%1 °").arg(gyrocompass.AngleHead));
        }
        else if(buf.contains("$PASHR"))
        {//	＄PASHR,POS,n,qq,hhmmss.ss,ddmm.mmmmmm,s,dddmm.mmmmmm,s,saaaaa.aaa
         //  ,,ttt.tt(航迹向),ggg.gg（地速）,svvv.vv（垂直速度）,pp.p,hh.h,vv.v,tt.t,vvvv*cc<CR><LF>
            ui->plainTextEdit_Serial_Reciive->appendPlainText("接受来自卫星导航系统的数据");
            for(int i=0;i<18;i++)
            {
                index[i]=GetComma((i+1),buf);
                qDebug()<<QString("第%1个逗号位置：").arg(i)<<index[i];
            }
            gps.qq=buf.mid(index[2],index[3]-index[2]-1).toInt();
            gps.hour=buf.mid(index[3],2).toInt();
            gps.minute=buf.mid((index[3]+2),2).toInt();
            gps.second=buf.mid((index[3]+4),index[4]-index[3]-5).toDouble();
            gps.latitude_du=buf.mid(index[4],2).toInt();
            gps.latitude_fen=buf.mid(index[4]+2,9).toDouble();
            gps.latitude_sign=buf[index[5]];
            gps.longtitude_du=buf.mid(index[6],3).toInt();
            gps.longtitude_fen=buf.mid((index[6]+3),9).toDouble();
            gps.height=buf.mid((index[8]+1),index[9]-index[8]-2).toDouble();
            gps.angleHead=buf.mid(index[10],index[11]-index[10]-1).toDouble();
            gps.speedToGround=buf.mid(index[11],index[12]-index[11]-1).toDouble();
            gps.speedVertical=buf.mid((index[12]+1),index[13]-index[12]-2).toDouble();
            qDebug()<<gps.qq;
            qDebug()<<gps.hour;
            qDebug()<<gps.minute;
            qDebug()<<gps.second;
            qDebug()<<gps.latitude_du;
            qDebug()<<QString::number(gps.latitude_fen,'g',9).append("°");//输出为字符串形式"33.445566"
            qDebug()<<gps.longtitude_du;
            qDebug("%.6f",gps.longtitude_fen);//输出为数字形式44.556677
            qDebug("%.3f",gps.height);
            qDebug("%.2f",buf.mid(index[10],6).toDouble());
            qDebug("%.2f",buf.mid(index[11],6).toDouble());
            qDebug("%.2f",buf.mid((index[12]+1),6).toDouble());
            qDebug("%.1f",buf.mid((index[13]),4).toDouble());
            qDebug("%.1f",buf.mid((index[14]),4).toDouble());
            qDebug("%.1f",buf.mid((index[15]),4).toDouble());

            ui->lineEdit_GPS_Latitude->setText(QString("%1° ").arg(gps.latitude_du)+QString::number(gps.latitude_fen,'g',9).append("'")+gps.latitude_sign);
            ui->lineEdit_GPS_Longitude->setText(QString("%1° ").arg(gps.longtitude_du)+QString::number(gps.longtitude_fen,'g',9).append("'"));
            ui->lineEdit_GPS_AngleTracks->setText(QString::number(gps.angleHead,'g',6).append("°"));
            ui->lineEdit_GPS_SpeedGround->setText(QString::number(gps.speedToGround,'g',6).append("kn"));
            ui->lineEdit_GPS_SpeedVertical->setText(QString::number(gps.speedVertical,'g',6).append("分米/秒"));

        }
        else
        {
            ui->plainTextEdit_Serial_Reciive->appendPlainText("出错了：信息识别错误");
        }


        if(!buf.isEmpty())
        {
            ui->plainTextEdit_Serial_Reciive->appendPlainText(buf);
        }
     }
     else
     {
        qDebug()<<"接受数据出错" + QString::number(mserial->bytesAvailable());
     }

}

void MainWindow::on_pushButton_Serial_Open_clicked()
{
    if(serialopen()==true)
    {
       qDebug()<<"串口打开";
       ui->plainTextEdit_Serial_Reciive->appendPlainText("串口打开："+ui->comboBox_SerialPort->currentText());
       ui->pushButton_Serial_Open->setEnabled(false);
       ui->pushButton_Serial_Close->setEnabled(true);
       ui->pushButton_Serial_Send->setEnabled(true);
       SerialState->setText("串口连接状态：已连接"+ui->comboBox_SerialPort->currentText());
    }
}

void MainWindow::on_pushButton_Serial_Close_clicked()
{
    qDebug()<<"串口关闭";
    mserial->close();
    ui->plainTextEdit_Serial_Reciive->appendPlainText("串口关闭："+ui->comboBox_SerialPort->currentText());
    ui->pushButton_Serial_Open->setEnabled(true);
    ui->pushButton_Serial_Close->setEnabled(false);
    ui->pushButton_Serial_Send->setEnabled(false);
    SerialState->setText("串口连接状态：已断开连接");
}

void MainWindow::on_pushButton_Serial_Clear_clicked()
{
    ui->plainTextEdit_Serial_Reciive->clear();
}

void MainWindow::on_pushButton_Serial_Refresh_clicked()
{
    serialportscan();
}

void MainWindow::on_pushButton_Serial_Send_clicked()
{
    if(mserial->isOpen())
        {
           QByteArray sendData = ui->lineEdit_Serial_Send->text().toUtf8();
           mserial->write(sendData);
        }
}
