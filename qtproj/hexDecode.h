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
    bool exist = false;

    uint32_t lenth;
    uint32_t address;
    uint32_t extendLinearAddress;
    uint32_t dataType;

    bool beginDownloadState;
    uint32_t packetNum;
    uint32_t packetSize;
    uint32_t shakeSuccessTime;

    uint32_t writeSuccessTime;
    QList<bool> hexPacketoK;

    char writeFlashCmd;
    enum bmsCmdType {
        READ_BOOT_CODE_INF = 0x10,
        READ_IC_INF = 0x51,
        HEX_INFO,
        ENTER_BOOTMODE,
        WRITE_FLASH,
        READ_FLASH,
    };


    QByteArray n00dataArray;
    QByteArray n01endArray;
    QByteArray n02extendArray;
    QByteArray n03startArray;
    QByteArray n04extendLinearArray;
    QByteArray n05startLinearArray;

    QList<uint32_t> hexArray;

    bool OpenHexFile(QFile *file, QString fileUrl);
    QString ReadHexFile(QFile *file);
    void Clear(void);
    QString packetToSendString(bmsCmdType cmdType, uint32_t packetNumber = 0);
    static bool isDownLoadCmd(char cmd);

private:
    uint32_t toUInt(QByteArray str);
};
