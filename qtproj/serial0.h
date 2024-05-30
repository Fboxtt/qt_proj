#ifndef SERIAL_H
#define SERIAL_H

#endif // SERIAL_H
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>

#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
class serial
{

public:
    serial();
    void Init(Ui::Widget *ui);
    void ClickOpenSerPort(Ui::Widget *ui);
private:
    QSerialPort SerialPort;

};
