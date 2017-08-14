#include "widget.h"
#include "ui_widget.h"
#include "qextserialport.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    initctrls();
}
void Widget::initctrls()
{
  // ui->recvTextBrowser->setText(port->portName());
   this->setWindowTitle("Myrobot");



}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_openPortBtn_clicked()
{

    QString portName = ui->portNameCombox->currentText();

}

void Widget::on_closePortBtn_clicked()
{

}

void Widget::on_sendBtn_clicked()
{

}
