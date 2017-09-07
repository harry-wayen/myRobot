#include "dialog.h"
#include "ui_dialog.h"
#include<string.h>
#include<QDebug>
#include<QTime>
#include<QtCore/qmath.h>
#include<QtSerialPort/QSerialPortInfo>
#include<QtSerialPort/QSerialPort>
#define PI 3.1415926
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    init();
}
void Dialog::init()   //初始化
{
    this->setWindowTitle("My root");
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->portNameComBox->addItem(info.portName());
    }
    ui->angle_0->setText(QString::number(90,10));
    ui->angle_1->setText(QString::number(90,10));
    ui->angle_2->setText(QString::number(90,10));
    ui->angle_3->setText(QString::number(90,10));
    ui->angle_4->setText(QString::number(90,10));
    ui->angle_5->setText(QString::number(90,10));
    ui->positionZ->setText(QString::number(0,10));

    ui->Slider_0->setMaximum(180);
    ui->Slider_0->setMinimum(0);
    ui->Slider_0->setValue(90);

    QTime t;
    t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);//初始化伪随机数种子


}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::read_data()    //调试用测试函数
{

    qDebug()<<"write";
   /* QByteArray recvdata = com->readAll();
    qDebug()<<recvdata.length();
    if(recvdata.length())
    {
        qDebug()<<recvdata;
        ui->textBrowser->insertPlainText(recvdata);
    }*/
}

void Dialog::on_openPortBtn_clicked()     //打开串口
{
    com->open(QIODevice::ReadWrite);
    com->setPortName(ui->portNameComBox->currentText());//设置串口名

    qint32 BaudeRate_index = ui->baudeRateCombox->currentIndex();
    switch(BaudeRate_index)
    {
    case 0:com->setBaudRate(QSerialPort::Baud4800);break;
    case 1:com->setBaudRate(QSerialPort::Baud9600);break;
    case 2:com->setBaudRate(QSerialPort::Baud115200);break;
    default:break;
    }
    com->setDataBits(QSerialPort::Data8);
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->setParity(QSerialPort::NoParity);
   // com->setStopBits(QSerialPort::OneStop);

    com->close();
    if(com->open(QIODevice::ReadWrite))
    {
        qDebug()<<"打开串口成功";
         connect(com,SIGNAL(bytesWritten(qint64)),this,SLOT(read_data()));
    }
}

void Dialog::on_sndCmdBtn_clicked()     //发送命令
{  // com->open(QIODevice::ReadWrite);
    if(!com->isOpen())
    {
        qDebug()<<"serial is no open";
    }
    com->setDataTerminalReady(true);
   // qDebug()<<com->portName();
    QString snddata = ui->cmdText->toPlainText().toLatin1();
    QByteArray data;
    data.append(snddata+"\r\n");

    qint64 len = com->write(data);
    qDebug()<<len;
    if(len == -1)
    {
        qDebug()<<"发送失败";
    }
    data.clear();
   // qDebug()<<data;
}

void Dialog::on_closePortBtn_clicked()
{
    com->close();
    qDebug()<<"串口已关闭";
}

void Dialog::on_sndAngleBtn_clicked()    //发送所有角度指令
{
    QByteArray sndop;
    int Robotnum = 6;
    int offSet[6];
    memset(offSet,0x00,6);
    for(int i = 0; i<Robotnum;++i)
    {
        int Angle;
        switch(i)
        {
        case 0:Angle = ui->angle_0->text().toInt();break;
        case 1:Angle = ui->angle_1->text().toInt();break;
        case 2:Angle = ui->angle_2->text().toInt();break;
        case 3:Angle = ui->angle_3->text().toInt();break;
        case 4:Angle = ui->angle_4->text().toInt();break;
        case 5:Angle = ui->angle_5->text().toInt();break;
        default:Angle = 90;break;
        }
        offSet[i] = 500+Angle*2000/180;
    }
     //QString opstr = "";
     QString opstr = QString("#0 P%1 #1 P%2 #2 P%3 #3 P%4 #4 P%5 #5 P%6 T2000").arg(QString::number(offSet[0],10),QString::number(offSet[1],10),
          QString::number(offSet[2],10),QString::number(offSet[3],10),QString::number(offSet[4],10),QString::number(offSet[5],10));
     /*while(offSet)
     {
        opstr.append(QString("#"))
     }*/
     qDebug()<<opstr;
     sndop.append(opstr+"\r\n");
     snddata(sndop);

}
void Dialog::sleep(unsigned int time)   //自定义延时函数，延时若干毫秒
{
    QTime reachtime = QTime::currentTime().addMSecs(time);
    while(QTime::currentTime() < reachtime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
void Dialog::on_op_catch_clicked()    //组合抓取正前方并放回动作
{
    on_op_bend_clicked();  //弯曲
    sleep(2000);           //自定义延时函数，延时500毫秒
    on_op_grap_clicked();//抓取
    sleep(2000);
    on_op_withdraw_clicked(); //撤回
    sleep(2000);
    on_pop_place_clicked();  //放置指定地方
    sleep(2000);
    on_up_down_clicked();     //松开

}
void Dialog::snddata(QByteArray &data)//把数据写到串口
{
    qint64 len = com->write(data);
    qDebug()<<len;
    if(len == -1)
    {
        qDebug()<<"发送失败";
    }
    data.clear();

}



void Dialog::on_op_Reset_clicked()    //复位
{
    QByteArray sndop;
    QString opstr = " #0 P1500 #1 P1500 #2 P1500 #3 P1500 #4 P1500 #5 P1500 T2000";
    sndop.append(opstr+"\r\n");
    snddata(sndop);


}

void Dialog::on_op_bend_clicked()   //弯曲
{

    QByteArray sndop;
    QString opstr;
    int destance = ui->destanceCombox->currentText().toInt();
    qDebug()<<destance;
    switch (destance) {
    case 1:   opstr = "#1 P1781 #2 P2141 #3 P2500 #5 P2500 T2000 ";
        break;
    case 2:   opstr = "#1 P1812 #2 P2053 #3 P2500 #5 P2500 T2000";
        break;
    case 3:   opstr = "#1 P1737 #2 P2285 #3 P2261 #5 P2500 T2000";
        break;
    case 4:   opstr = "#1 P1797 #2 P2165 #3 P2261 #5 P2500 T2000";
        break;
    case 5:   opstr = "#1 P1729 #2 P2500 #3 P1900 #5 P2500 T2000";
        break;
    case 6:   opstr = "#1 P1785 #2 P2447 #3 P1850 #5 P2500 T2000";
        break;
    case 7:   opstr = "#1 P1838 #2 P2328 #3 P1827 #5 P2500 T2000 ";
        break;
    case 8:   opstr = "#1 P1928 #2 P2202 #3 P1818 #5 P2500 T2000 ";
        break;
    case 9:   opstr = "#1 P2353 #2 P1078 #3 P2500 #5 P2500 T2000";
        break;
    case 10:   opstr ="#1 P2261 #2 P1435 #3 P2164 #5 P2500 T2000";
        break;
    default: opstr = "#1 P1700 #2 P1885 #3 P1883 #4 P1500 #5 P2500 T2000";
        break;
    }
    sndop.append(opstr+"\r\n");
    snddata(sndop);
}

void Dialog::on_op_withdraw_clicked()   //撤回
{
    QByteArray sndop;
    QString opstr = "#1 P1500 #2 P1885 #3 P1883 T2000";
    sndop.append(opstr+"\r\n");
    snddata(sndop);

}

void Dialog::on_op_grap_clicked()   //抓取
{
    QByteArray sndop;
    QString opstr = "#5 P1660 T2000";
    sndop.append(opstr+"\r\n");
    snddata(sndop);
}

void Dialog::on_pop_place_clicked()   //转至定点
{
    QByteArray sndop;
    QString opstr = "#0 P500 T2000";
    sndop.append(opstr+"\r\n");
    snddata(sndop);
}

void Dialog::on_up_down_clicked()    //松开
{
    QByteArray sndop;
    QString opstr = "#5 P2500 T2000";
    sndop.append(opstr+"\r\n");
    snddata(sndop);
}
void Dialog::on_sndPosiBtn_clicked()     //对坐标指令进行处理
{/*  实现方法一（空间任意抓取）：尚未调试完成
    int flag = 1;
    QByteArray sndop;
    qreal l1 = 10.5;
    qreal l2 = 6.0;
    qreal l3 = 16.5;
    qreal Px = ui->PositionX->text().toDouble();
    qreal Py = ui->PositionY->text().toDouble();
    qreal Pz = ui->positionZ->text().toDouble();
    //int offset1 = 0,offset2 = 0,offset3 = 0;
    qreal Pl = sqrt(Py*Py + Px*Px);
    qreal radian = qAtan2(Py,Px);
    int offSet = 500+abs(radian*2000/PI);
    if(offSet<500 ||offSet>2500)
    {
        qDebug()<<"舵机偏移量超出限制";
    }
    qDebug()<<"offset = "<<offSet;
    QString opstr = QString("#0 P%1 T2000").arg(QString::number(offSet,10));
    qDebug()<<"opstr = "<<opstr;
    sndop.append(opstr+"\r\n");
    snddata(sndop);
    sleep(2000);
    while(flag)
    {
    int a1 = 90+qrand()%90;
    int a2 = 90+qrand()%90;
    int a3 = 90+qrand()%90;
    qDebug()<<"a1="<<a1;
    qDebug()<<"a2="<<a2;
    qDebug()<<"a3="<<a3;
    qreal dz = 9+l1*qSin((qreal)(a1))+l2*qSin((qreal)(a1-90+(180-a2)))+l3*qSin((qreal)(a1+a2+(180-a3)));//底座高9cm
    qreal dl = fabs(l1*qCos((qreal)(a1)))+fabs(l2*qCos((qreal)(a1-90+(180-a2))))+fabs(l3*qCos((qreal)(a1-90+(180-a2)-90+(180-a3))));
  //qreal dl = l1*qCos((qreal)(a1))+l2*qCos()
    qDebug()<<"l3*qSin((qreal)(a1+a2+(180-a3))) = "<<l3*qSin((qreal)(a1+a2+(180-a3)));
    qDebug()<<"l2*qSin((qreal)(a1-90+(180-a2)))= "<<l2*qSin((qreal)(a1-90+(180-a2)));
    qDebug()<<"l1*qSin((qreal)(a1)) = "<<l1*qSin((qreal)(a1));
    qDebug()<<"fabs(l1*qCos((qreal)(a1))) = "<<fabs(l1*qCos((qreal)(a1)));
    qDebug()<<""
    qDebug()<<"z="<<dz;
    qDebug()<<"l="<<dl;
    if(fabs(dz-Pz) <= 0.5 && fabs(dl-Pl)<= 0.5)
    {

        qDebug()<<" match success"<<endl;
        ui->angle_1->setText(QString::number(a1,10));
        sleep(2000);
        ui->angle_2->setText(QString::number(a2,10));
        sleep(2000);
        ui->angle_3->setText(QString::number(a3,10));
        sleep(2000);
        flag = 0;
    }
    }
    on_op_relax_clicked();
    sleep(1000);
    //松开
    on_op_grap_clicked();//抓取
    sleep(2000);

    on_op_withdraw_clicked();//撤回
    sleep(2000);
    on_pop_place_clicked();//转至定点
    sleep(2000);
    on_up_down_clicked();//放下
    sleep(2000);*/




//实现方法二：已调试完成
    QByteArray sndop;
    qreal Px = ui->PositionX->text().toDouble();
    qreal Py = ui->PositionY->text().toDouble();
    qreal Pz = ui->positionZ->text().toDouble();
    qreal radian = qAtan2(Py,Px);

    int offSet = 500+abs(radian*2000/PI);
    if(offSet<500 ||offSet>2500)
    {
        qDebug()<<"舵机偏移量超出限制";
    }
    qDebug()<<"offset = "<<offSet;
    QString opstr = QString("#0 P%1 T2000").arg(QString::number(offSet,10));
    qDebug()<<"opstr = "<<opstr;
    sndop.append(opstr+"\r\n");
    snddata(sndop);
    sleep(2000);
    qreal destance = sqrt( Py*Py + Px*Px);
    int des = (destance-8)/2;
    ui->destanceCombox->setCurrentText(QString::number(des,10));
    qDebug()<<"des="<<des;
    on_op_catch_clicked();
}



void Dialog::on_Slider_0_sliderMoved(int position) //用滑动条控制零号舵机
{
    int value = ui->Slider_0->value();
    ui->Slider_0text->setText(QString::number(value,10));
    int offSet = 500 + abs(value*2000/180);
    QString opstr = QString("#0 P%1 T2000").arg( QString::number(offSet));
    QByteArray data;
    data.append(opstr + "\r\n");
    snddata(data);
}

void Dialog::on_Slider_0_valueChanged(int value)//用滑动条控制零号舵机
{
    on_Slider_0_sliderMoved(ui->Slider_0text->text().toInt());
}

void Dialog::on_angle_0_textChanged(const QString &arg1) //自动发送角度
{
    sndAngle(arg1,0);
}

void Dialog::sndAngle(const QString angle,const int index)   //发送角度
{
    int robotIndex = index;
    int agl = angle.toInt();
    int offset = 500 + abs(agl*2000/180);
    QString opstr = QString("#%1 P%2 T1500").arg(QString::number(robotIndex),QString::number(offset));
    QByteArray data;
    data.append(opstr + "\r\n");
    snddata(data);
}

void Dialog::on_angle_1_textChanged(const QString &arg1)
{
     sndAngle(arg1,1);
}

void Dialog::on_angle_2_textChanged(const QString &arg1)
{
     sndAngle(arg1,2);
}

void Dialog::on_angle_3_textChanged(const QString &arg1)
{
     sndAngle(arg1,3);
}

void Dialog::on_angle_4_textChanged(const QString &arg1)
{
     sndAngle(arg1,4);
}

void Dialog::on_angle_5_textChanged(const QString &arg1)
{
     sndAngle(arg1,5);
}

void Dialog::on_op_relax_clicked()
{
    QByteArray sndop;
    QString opstr = "#5 P2500 T1000";
    sndop.append(opstr+"\r\n");
    snddata(sndop);
}
