#ifndef HEXDECODE_H

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

#define HEXDECODE_H

#endif // HEXDECODE_H

class hexDecode {

public:
    hexDecode();
//    ~hexDecode();
    uint32_t lenth;
    uint32_t address;
    uint32_t dataType;

    QByteArray dataArray;
    QByteArray endArray;
    QByteArray extendArray;
    QByteArray startArray;
    QByteArray extendLinearArray;
    QByteArray startLinearArray;

    QList<uint32_t> hexArray;

    bool OpenHexFile(QFile *file, QString fileUrl);
    void ReadHexFile(QFile *file);
};
