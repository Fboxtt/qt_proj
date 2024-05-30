#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <serial0.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    serial se;
    se.Init(ui);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    QString str = ui->plainTextEdit->toPlainText();
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
    serial se;
    se.RefreshSerial(ui);
}

void Widget::on_openBtn_clicked()
{
    serial se;
    qDebug() << "in open serial";
//    SelAndOpenSerial(NULL,NULL);
    se.ClickOpenSerPort(ui);
    qDebug() << "out open serial";
}
