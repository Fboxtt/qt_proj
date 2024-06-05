#ifndef TEXTDECODE_H
#define TEXTDECODE_H

#endif // TEXTDECODE_H

#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
//#include "serial0.h"
#include <QTextBlock>
#include <Qmap>
#include <QTime>
#include <QVector>

class textDcode
{

public:
    textDcode();

    QString DecodeHexToCommand(Ui::Widget *ui);
    QString SendCmdDocode(QStringList hexList, QString decodeStr);
    QString ByteDecode(QMap<uint32_t, QString> mapCode, uint8_t keys);
    QString SendDataDecode(Ui::Widget *ui, QString decodeStr);
    QString readDataDocode(QStringList hexStrLis, QString decodeStr);
    uint32_t TbsDecode(QVector<uint8_t> hexVector, uint32_t dataSum);

    QMap<uint32_t, QString> typeCode;
    QMap<uint32_t, QString> funcCode;
    QMap<uint32_t, QString> ackCode;
private:

};
