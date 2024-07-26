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
#include <QTime>
class serial
{

public:
    serial();
    void Init(Ui::Widget *ui);
    void ClickOpenSerPort(Ui::Widget *ui);
    void RefreshSerial(Ui::Widget *ui);
    void serial_Read(Ui::Widget *ui, QTimer *tim);
    QString SerialSend(Ui::Widget *ui, QString Data);
    void SerialSend(Ui::Widget *ui, QByteArray Data);
    QSerialPort SerialPort;
private:

public:
    void TimeOut(QTimer *tim);

    enum BAT_COMMAND_SEND_STATUS {
        COMPLETE,
        INCOMPLETE,
        FIRST_CONNECT,
    };

    BAT_COMMAND_SEND_STATUS batComSendStatus = FIRST_CONNECT;
};
