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
#include "currency.h"

#define HEXDECODE_H

#endif // HEXDECODE_H

#define SHAKE_TIME_LIMIT 3

class hexDecode {

public:
    hexDecode();
//    ~hexDecode();
    bool exist = false;

    uint32_t hexLenth;
    uint32_t address;
    uint32_t extendLinearAddress;
    uint32_t dataType;
    bool eraseFlag;
    bool beginDownloadState;
    uint32_t packetId;
    uint32_t packetSize = 512;
    uint32_t packetNum;
    uint32_t shakeSuccessTime;

    uint32_t writeSuccessTime;
    QList<bool> hexPacketoK;

    int packetNumLErr = 0;
    int bmsNack = 0;
    int cmdTypeErr = 0;

    char writeFlashCmd;

    QTime downloadStartTim;
    enum bmsCmdType {
        READ_BOOT_CODE_INF = 0x10,
        READ_IC_INF = 0x51,
        HEX_INFO,
        ENTER_BOOTMODE,
        EARSE_ALL,
        WRITE_FLASH,
        READ_FLASH,
    };
    enum Download_ERR {
        DOWNLOAD_OK = true,
        PACKET_NUM_LENTH_ERR,
        DOWNLOAD_DONE,
        BMS_NACK,
        CMD_TYPE_ERR,

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
    uint8_t DownLoadProcess(textStruct text, QString* outPutStr);
    uint32_t litBytetoUInt(QByteArray inputArray);
    void DownloadClear(void);
    void AllClear(void);
    bool isErrExceeding(void);
    QString DownLoadLog(void);
private:
    uint32_t toUInt(QByteArray str);
};
