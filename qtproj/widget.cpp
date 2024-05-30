#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <serial0.h>

serial se;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // serial se;
    se.Init(ui);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    QString str = ui->sendData->toPlainText();
//    QString str = ui->lineEdit->text();
    qDebug() << str;
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
//extern int SelAndOpenSerial(int argc, char *argv[]);
void Widget::on_conectSerialButton_clicked()
{

}

void Widget::on_searchBtn_clicked()
{
    // serial se;
    se.RefreshSerial(ui);
}

void Widget::on_openBtn_clicked()
{
    // serial se;
    qDebug() << "open serial btn";
    se.ClickOpenSerPort(ui);
//    connect()
//    QObject::connect(se.SerialPort, SIGNAL(QSerialPort::readyRead), this, SIGNAL(Widget::SerialPortReadyRead_slot));
//    QSerialPort* serial0 = new QSerialPort(this);
    /* 创建接收数据信号槽 */
    connect(&se.SerialPort, &QSerialPort::readyRead, this, &Widget::SerialPortReadyRead_slot);
//    connect(ui->rateBox, QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){
//        if (index == 8) {
//            ui->rateBox->setEditable(true);
//            ui->rateBox->setCurrentText(NULL);
//        } else {
//            ui->rateBox->setEditable(false);
//        }
//    });
}

void Widget::SerialPortReadyRead_slot()
{
    qDebug() << "in ready slot";
    se.serial_Read(ui);
}

void Widget::on_sendBox_clicked()
{
    // serial se;
    se.on_sendBox_clicked(ui);
}

void Widget::on_clearReceiveDataButton_clicked()
{
    se.serial_Read(ui);
}
