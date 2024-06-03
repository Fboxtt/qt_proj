#include "widget.h"
#include "ui_widget.h"
#include "textDecode0.h"
#include <QDebug>
#include <QFileDialog>
#include <serial0.h>


serial se;
textDcode dcode;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    se.Init(ui);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    dcode.DecodeHexToCommand(ui);
}


void Widget::on_pushButton_clicked()
{

}

void Widget::on_selectFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open hex"), "/home/jana", tr("(*.hex)"));
    if (fileName.isEmpty()) {
//        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        ui->lineEdit->setText("未选择文件");
    }
    else {
        ui->lineEdit->setText(fileName);
    }
}

void Widget::on_conectSerialButton_clicked()
{

}

void Widget::on_searchBtn_clicked()
{
    // serial se;
    se.RefreshSerial(ui);
}


void Widget::onTimeOut()
{
    se.TimeOut(ui,tim);
}

void Widget::on_openBtn_clicked()
{
    qDebug() << "open serial btn";
    se.ClickOpenSerPort(ui);
    /* 创建接收数据信号槽 */
    connect(&se.SerialPort, &QSerialPort::readyRead, this, &Widget::SerialPortReadyRead_slot);

    tim = new QTimer();
    tim->setInterval(100);
    connect(tim, SIGNAL(timeout()),this,SLOT(onTimeOut()));
}

void Widget::SerialPortReadyRead_slot()
{
    qDebug() << "in ready slot";
    se.serial_Read(ui,tim);
}

void Widget::on_sendBox_clicked()
{
    // serial se;
    se.on_sendBox_clicked(ui);
}

void Widget::on_clearReceiveDataButton_clicked()
{
    ui->receiveData->clear();
}
