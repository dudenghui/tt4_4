#include "mainwindow.h"
#include "ui_mainwindow.h"


/*************************************以下为数据库操作部分部分***********************************************/
void MainWindow::on_timer_timeout()
{
    on_pushButton_DB_add_clicked();
}

void MainWindow::on_pushButton_DB_open_clicked()
{//打开数据表
//    QString aFile=QFileDialog::getOpenFileName(this,"选择数据库文件","","SQL Lite数据库(*.db *.db3)");
//    if (aFile.isEmpty())  //选择SQL Lite数据库文件
//       return;

    QString runPath = QCoreApplication::applicationDirPath();//获取当前目录的绝对路径
    runPath.append("/navigation_DB.db");//把数据库文件放在运行目录下
//    QString runPath =":/database/navigation_DB.db";//获取相对路径（方法错误）
    qDebug()<<"文件路径"<<runPath;
//打开数据库
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    DB.setDatabaseName(runPath); //设置数据库名称
    if (!DB.open())   //打开数据库
    {
        qDebug()<<"打开数据库错误";
        return;
    }

    ui->pushButton_DB_add->setEnabled(true);
    ui->pushButton_DB_close->setEnabled(true);
    ui->pushButton_DB_delete->setEnabled(true);
    ui->pushButton_DB_insert->setEnabled(true);
    ui->pushButton_DB_save->setEnabled(true);

    QStringList table_list;
    table_list<<"zongdao"<<"GPS"<<"tuoluo"<<"jichengyi";
    ui->comboBox_DB_table->addItems(table_list);
}

void MainWindow::on_pushButton_DB_close_clicked()
{

}

void MainWindow::on_comboBox_DB_table_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox_DB_table->currentText()=="zongdao")
    {
        qDebug()<<"显示综导台数据表";
        tabModel=new QSqlTableModel(this,DB);//数据表

        tabModel->setTable("zongdao"); //设置数据表
        tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
        tabModel->setSort(tabModel->fieldIndex("RecNo"),Qt::AscendingOrder); //排序
        if (!(tabModel->select()))//查询数据
        {
           qDebug()<<"数据查询失败";
           return;
        }

    //字段显示名
        tabModel->setHeaderData(tabModel->fieldIndex("Number"),Qt::Horizontal,"序号");
        tabModel->setHeaderData(tabModel->fieldIndex("Time"),Qt::Horizontal,"时间");
        tabModel->setHeaderData(tabModel->fieldIndex("Longitude"),Qt::Horizontal,"经度");
        tabModel->setHeaderData(tabModel->fieldIndex("Latitude"),Qt::Horizontal,"纬度");
        tabModel->setHeaderData(tabModel->fieldIndex("EastSpeed"),Qt::Horizontal,"东向速度");
        tabModel->setHeaderData(tabModel->fieldIndex("NorthSpeed"),Qt::Horizontal,"北向速度");
        tabModel->setHeaderData(tabModel->fieldIndex("Speed"),Qt::Horizontal,"航速");
        tabModel->setHeaderData(tabModel->fieldIndex("VerticalSpeed"),Qt::Horizontal,"升沉速度");
        tabModel->setHeaderData(tabModel->fieldIndex("Depth"),Qt::Horizontal,"下潜深度");
        tabModel->setHeaderData(tabModel->fieldIndex("DepthBelow"),Qt::Horizontal,"艇下深");
        tabModel->setHeaderData(tabModel->fieldIndex("DepthSea"),Qt::Horizontal,"海深");
        tabModel->setHeaderData(tabModel->fieldIndex("SeaSituation"),Qt::Horizontal,"海况");
        tabModel->setHeaderData(tabModel->fieldIndex("AngleHead"),Qt::Horizontal,"艏向角");
        tabModel->setHeaderData(tabModel->fieldIndex("AngleTrack"),Qt::Horizontal,"航迹角");

        ui->tableView->setModel(tabModel);//设置数据模型
    }
    else if(ui->comboBox_DB_table->currentText()=="GPS")
    {
        qDebug()<<"显示GPS数据表";
        tabModel=new QSqlTableModel(this,DB);//数据表

        tabModel->setTable("GPS"); //设置数据表
        tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
        tabModel->setSort(tabModel->fieldIndex("RecNo"),Qt::AscendingOrder); //排序
        if (!(tabModel->select()))//查询数据
        {
           qDebug()<<"数据查询失败";
           return;
        }

    //字段显示名
        tabModel->setHeaderData(tabModel->fieldIndex("Number"),Qt::Horizontal,"序号");
        tabModel->setHeaderData(tabModel->fieldIndex("Time"),Qt::Horizontal,"时间");
        tabModel->setHeaderData(tabModel->fieldIndex("Longitude"),Qt::Horizontal,"经度");
        tabModel->setHeaderData(tabModel->fieldIndex("Latitude"),Qt::Horizontal,"纬度");
        tabModel->setHeaderData(tabModel->fieldIndex("AngleTrack"),Qt::Horizontal,"航迹角");
        tabModel->setHeaderData(tabModel->fieldIndex("SpeedToGround"),Qt::Horizontal,"对地速度");
        tabModel->setHeaderData(tabModel->fieldIndex("SpeedVertical"),Qt::Horizontal,"升沉速度");

        ui->tableView->setModel(tabModel);//设置数据模型
    }
    else if(ui->comboBox_DB_table->currentText()=="tuoluo")
    {
        qDebug()<<"显示陀螺罗经数据表";
        tabModel=new QSqlTableModel(this,DB);//数据表

        tabModel->setTable("tuoluo"); //设置数据表
        tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
        tabModel->setSort(tabModel->fieldIndex("RecNo"),Qt::AscendingOrder); //排序
        if (!(tabModel->select()))//查询数据
        {
           qDebug()<<"数据查询失败";
           return;
        }

    //字段显示名
        tabModel->setHeaderData(tabModel->fieldIndex("Number"),Qt::Horizontal,"序号");
        tabModel->setHeaderData(tabModel->fieldIndex("Time"),Qt::Horizontal,"时间");
        tabModel->setHeaderData(tabModel->fieldIndex("AngleHead"),Qt::Horizontal,"艏向角");

        ui->tableView->setModel(tabModel);//设置数据模型
    }
    else if(ui->comboBox_DB_table->currentText()=="jichengyi")
    {
        qDebug()<<"显示计程仪数据表";
        tabModel=new QSqlTableModel(this,DB);//数据表

        tabModel->setTable("jichengyi"); //设置数据表
        tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
        tabModel->setSort(tabModel->fieldIndex("RecNo"),Qt::AscendingOrder); //排序
        if (!(tabModel->select()))//查询数据
        {
           qDebug()<<"数据查询失败";
           return;
        }

    //字段显示名
        tabModel->setHeaderData(tabModel->fieldIndex("Number"),Qt::Horizontal,"序号");
        tabModel->setHeaderData(tabModel->fieldIndex("Time"),Qt::Horizontal,"时间");
        tabModel->setHeaderData(tabModel->fieldIndex("Speed"),Qt::Horizontal,"速度");
        tabModel->setHeaderData(tabModel->fieldIndex("Mileage"),Qt::Horizontal,"里程");

        ui->tableView->setModel(tabModel);//设置数据模型
    }
}

void MainWindow::on_spinBox_DB_time_valueChanged(int arg1)
{//定时时间变化
    fTimer->setInterval(arg1);
}

void MainWindow::on_pushButton_DB_add_clicked()
{//添加记录
    tabModel->insertRow(tabModel->rowCount(),QModelIndex()); //在末尾添加一个记录
    int currow=tabModel->rowCount()-1; //获得当前行
    QDate da=QDate::currentDate();
    QTime curtime=QTime::currentTime();
    QString date=da.toString("yyyy.MM.dd") ;
    date.append("  "+curtime.toString("H:m:s"));
    qDebug()<<"添加记录中";

    tabModel->setData(tabModel->index(currow,0),tabModel->rowCount()); //自动生成编号
    tabModel->setData(tabModel->index(currow,1),date); //当前日期

    if(ui->comboBox_DB_table->currentText()=="zongdao")
    {
        tabModel->setData(tabModel->index(currow,2),datadisp.zd_longitude); //
        tabModel->setData(tabModel->index(currow,3),datadisp.zd_latitude); //
        tabModel->setData(tabModel->index(currow,4),ui->lineEdit_zd_EastSpeed->text()); //
        tabModel->setData(tabModel->index(currow,5),ui->lineEdit_zd_NorthSpeed->text()); //
        tabModel->setData(tabModel->index(currow,6),ui->lineEdit_zd_ForwardSpeed->text()); //
        tabModel->setData(tabModel->index(currow,7),ui->lineEdit_zd_UpDownSpeed->text()); //
        tabModel->setData(tabModel->index(currow,8),ui->lineEdit_zd_Depth->text()); //
        tabModel->setData(tabModel->index(currow,9),ui->lineEdit_zd_DepthBelow->text()); //
        tabModel->setData(tabModel->index(currow,10),ui->lineEdit_zd_SeaDepth->text()); //
        tabModel->setData(tabModel->index(currow,11),ui->lineEdit_SeaSituation->text()); //
        tabModel->setData(tabModel->index(currow,12),datadisp.zd_anglehead); //
        tabModel->setData(tabModel->index(currow,13),datadisp.zd_angle_track); //

    }
    else if(ui->comboBox_DB_table->currentText()=="GPS")
    {
        tabModel->setData(tabModel->index(currow,2),222);
    }
    else if(ui->comboBox_DB_table->currentText()=="tuoluo")
    {
        tabModel->setData(tabModel->index(currow,2),333);
    }
    else if(ui->comboBox_DB_table->currentText()=="jichengyi")
    {
        tabModel->setData(tabModel->index(currow,2),444);
    }


    tabModel->submitAll();
    qDebug()<<"插入成功";

}

void MainWindow::on_pushButton_DB_delete_clicked()
{//删除记录
    tabModel->removeRow(tabModel->rowCount()-1); //删除最后一行
    tabModel->submitAll();
}

void MainWindow::on_pushButton_DB_insert_clicked()
{//插入记录
//    QModelIndex curIndex=ui->tableView->currentIndex();
//    tabModel->insertRow(curIndex.row(),QModelIndex());
//    tabModel->submitAll();
}

void MainWindow::on_pushButton_DB_save_clicked()
{//保存修改
    bool res=tabModel->submitAll();

    if (!res)
    {
        qDebug()<<"保存失败";
    }
    else
    {
        //ui->pushButton_save->setEnabled(false);
    }

}

void MainWindow::on_pushButton_DB_Counter_clicked()
{//定时发送
    if(ui->pushButton_DB_Counter->text()=="定时发送")
    {
        fTimer->start();
        ui->pushButton_DB_Counter->setText("关闭定时");
    }
    else
    {
        fTimer->stop();
        ui->pushButton_DB_Counter->setText("定时发送");
    }

}
