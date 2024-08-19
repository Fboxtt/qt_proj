#ifndef CURRENCY_H
#define CURRENCY_H

#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
//#include "serial0.h"
#include <QTextBlock>
#include <Qmap>
#include <QTime>
#include <QVector>
#include <QList>

class textStruct
{
public:
    enum sendState {
        SEND,
        RECEIVE,
    };
    enum dataErr {
        NO_ERR = 0,
        LENTH_ERR,
    };
    enum ACK_STATE {
        ACK_OK = 0x0,
        ACK_LENTH_ERR,
        ACK_TYPE_ERR,
        ACK_ID_ERR,
        ACK_SHAKE_ERR,
        ACK_CHECK_ERR,
        ACK_PACKET_ERR,
    };
    textStruct(){
        byteLenth = 0;
    }
    textStruct(QString text);
    QString tim;
    QString text;
    sendState sendOrReceive;
    dataErr Err;
    uint8_t ACK;
    uint8_t cmd;
    bool cmdOk;
    bool checkSumOk;
    bool lenthOk;
    int byteLenth;
    QByteArray dataArray;

};

#endif // CURRENCY_H
