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
#define SHAKE_BACKUP_TIME_LIMIT 3
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
    bool beginEraseState;
    uint32_t packetId;
    uint32_t packetSize = 500;
    uint32_t packetNum;
    uint32_t shakeSuccessTime;

    bool downloadBackupFlag = 0;
    uint32_t shakeBackupSuccTim = 0;

    uint32_t writeSuccessTime;
    QList<bool> hexPacketoK;

    int packetNumLErr = 0;
    int bmsNack = 0;
    int cmdTypeErr = 0;

    char writeFlashCmd;

    QTime downloadStartTim;
    enum bmsCmdType {
        READ_IC_INF         = 0x71,
//        HEX_INFO            = 0x72,
//        GET_BT_VERSION      = 0x53,

        DOWNLOAD_BUFFER     = 0x75,
        ENTER_BOOTMODE      = 0x76,
        WRITE_FLASH         = 0x77,
        REC_TOTAL_CHECKSUM  = 0x78,
        READ_FLASH          = 0x79,
        ENTER_APP           = 0x7A,
        
        DOWNLOAD_BACKUP     = 0x7C,
        RESTORE_BACKUP      = 0x7D,
    };
    enum Download_ERR {
        DOWNLOAD_OK = true,
        PACKET_NUM_LENTH_ERR,
        DOWNLOAD_DONE,
        BMS_NACK,
        CMD_TYPE_ERR,
        CHECKSUM_ACK,
        JUST_ERASE,
    };

    QByteArray n00dataArray;
    QByteArray n01endArray;
    QByteArray n02extendArray;
    QByteArray n03startArray;
    QByteArray n04extendLinearArray;
    QByteArray n05startLinearArray;

    QByteArray dataAll;
    bool mergeHexOk = false;

    QList<uint32_t> hexArray;

    QByteArray totalCheckSumArray;

    bool OpenHexFile(QFile *file, QString fileUrl);
    QString ReadHexFile(QFile *file);
    QByteArray CopyHexFile(QFile *file);
    void MergeHex(QStringList nameList);

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
