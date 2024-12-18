#ifndef TEXTDECODE_H
#define TEXTDECODE_H

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
class datTypDic
{
public:
    enum DATA_TYPE {
        ULONG,
        LONG,
        USHORT,
        SHORT,
        CHAR,
        UCHAR,
        STRING,
        WORD,
        DWORD,
        BYTE,
    };
    enum ENDIAN_TYPE {
        LITTLE,
        BIG,
    };
    enum SIGNED_TYPE {
        SIGNED,
        UNSIGNED,
    };

    datTypDic(DATA_TYPE type, QString typeName, uint32_t typeLenth, ENDIAN_TYPE endianType, SIGNED_TYPE signedType);
    DATA_TYPE type;
    QString typeName;
    uint32_t typeLenth;
    ENDIAN_TYPE endianType;
    SIGNED_TYPE signedType;
};

class tver
{

    public:
        // static uint32_t datInCmdAddr;
        // static uint32_t datLenth;
        tver(QString valName, datTypDic::DATA_TYPE dataType, uint32_t lenth);
        tver(QString valName, datTypDic::DATA_TYPE dataType);
        tver(){};
        QString valName;
        uint32_t unitInCmdAddr;
        uint32_t unitInDatAddr;
        datTypDic::DATA_TYPE dataType;
        datTypDic::ENDIAN_TYPE endianType;
        datTypDic::SIGNED_TYPE signedType;

        uint32_t typeLenth;
        uint32_t uintVal;
        QByteArray byteArray;
};
class dataCell
{

    public:
        dataCell(QString valName, datTypDic::DATA_TYPE dataType, uint32_t lenth);
        dataCell(QString valName, datTypDic::DATA_TYPE dataType);
        dataCell(){};
        QString valName;
        uint32_t unitInCmdAddr;
        uint32_t unitInDatAddr;
        datTypDic::DATA_TYPE dataType;
        datTypDic::ENDIAN_TYPE endianType;
        datTypDic::SIGNED_TYPE signedType;

        uint32_t typeLenth;
        uint32_t uintVal;
        QByteArray byteArray;
        QByteArray bigEndianBArray;

        QMap<uint32_t, QString> bitMap;
        uint32_t findBitVal(QString BitName);
};

class dataStruct
{
public:
    dataStruct();
    dataCell* value(QString valName);
    void insert(dataCell addCell);
    QString OutPutStru(void);
    QString displayData(void);
    QString csvName(void);
    QString csvData(void);
    QString strArray(void);
    bool newDataStatus;

    QList<QString> keyList;
    QList<dataCell> dataCellList;
    uint32_t dataLenth;
    QTime accurTime;
    char cmdType;
};

class caliStruct:public dataStruct
{
    using dataStruct::dataStruct;
    public:
        caliStruct();
};

class sysStruct:public dataStruct
{
    using dataStruct::dataStruct;
    public:
        sysStruct();
};

class snStruct:public dataStruct
{
    using dataStruct::dataStruct;
    public:
        snStruct(int);
};

class tverStruct
{
    public:
        tverStruct();
        tver value(QString valName);
        void insert(tver addTver);
        bool newDataStatus;

        QMap<QString, tver> tverMap;
        QList<QString> keyList;
        uint32_t dataLenth;
};


class tbs
{
public:
    tbs(QString valName, datTypDic::DATA_TYPE dataType);
    tbs(){};
    QString valName;
    uint32_t unitInCmdAddr;
    uint32_t unitInDatAddr;
    datTypDic::DATA_TYPE dataType;
    datTypDic::ENDIAN_TYPE endianType;
    datTypDic::SIGNED_TYPE signedType;

    uint32_t typeLenth;
    uint32_t uintVal;
//    void Init(QString* valName, uint32_t address, uint32_t cmdAddress);
};
class tbsStruct:public dataStruct
{
    using dataStruct::dataStruct;
    public:
        tbsStruct();
        void addStatusBits(void);
};


#define PC_GET_INF					0x71		// 获取BT版本号，APP版本号，BUFFER版本号，BACKUP版本号，芯片型号，芯片可写区域
#define PC_GET_BT_INF				0x72		// 获取BT详细版本号

#define PC_SET_DOWNLOAD_BUFFER		0x75		// 擦除所有APROM
#define PC_SHAKE_ENTER_BOOTMODE 	0x76		// 进入更新模式，即握手信号
#define PC_SET_WRITE_FLASH			0x77		// 更新程序命令
#define PC_SET_ALL_CHECKSUM        	0x78		// 发送校验和
#define PC_GET_READ_FLASH           0x79        // 读FLASH指定地址
#define BMS_SHAKE_ENTER_APP         0x7A        // 进入APP

#define PC_SET_DOWNLOAD_BACKUP			0x7C		// 下载备份
#define PC_SET_RESTORE_BACKUP			0x7D		// 将备份恢复到APP中

#define ERR_NO                  0x00        // 无异常
#define ERR_CMD_LEN             0x02        // 从机接收到的包长度和命令长度不对
#define ERR_CMD_ID              0x04        // 没有命令
#define ERR_CHKSUM               0x06        // 主机某个包校验和错误
#define ERR_OPERATE             0x07        // 未能完成主机要求的操作
#define ERR_SHAKEHAND			0x20 		// 握手次数错误
#define ERR_PACKET_NUMBER       0x21        // 主机包的序号跳错误
#define ERR_MEM_NOT_ENOUGH      0x22        // 主机hex文件过大无法写入
#define ERR_ALL_CHECK			0x23		// 总包校验和错误
#define ERR_REMAP			    0x24		// 重映射错误
#define ERR_AREA_BLANK			0x25		// 区域内数据为0
#define ERR_AREA_NOT_WRITABLE	0x26		// 区域不可写
#define ERR_DOWNLOAD_DONE		0x27		// 烧录已完成，请重新开始
#define ERR_ERASE				0x28		// 擦除错误

class textDcode
{

public:

    bool haveHex = false;
    bool legality = false;

    uint32_t actualLen = 0;
    QByteArray actualHex;

    uint8_t address = 0x00;

    uint8_t bmsType = 0;
    uint8_t cmd = 0;
    uint8_t no80Cmd = 0;

    QByteArray dataHex;
    uint32_t dataLen = 0;

    uint32_t checkSum = 0;
    uint32_t ack = 0;

    QByteArray fullHex;
    uint32_t fullLen = 0;

    QByteArray noPacketHex;
    uint32_t noPacketLen;

    bool SplitData(QByteArray);
    textDcode();

    QString PlainTextDecode(Ui::Widget *ui);
    QString SendCmdDocode(QStringList hexList, QString decodeStr);
    QString ByteDecode(QMap<uint32_t, QString> mapCode, uint8_t keys);
    QString AddTimeStamp(Ui::Widget *ui, QString decodeStr);
    QString readDataDocode(QStringList hexStrLis, QString decodeStr);
    uint32_t CalCheckSum(QVector<uint8_t> hexVector);
    void itemToTable(QVector<QTableWidgetItem>* itemTableList);
    void clearTableItem(QVector<QTableWidgetItem>* itemTableList);
    QVector<tbs> HexWriteTbs(QStringList dataList);
    void IntWriteTbs(QStringList dataList);
    QString HexWriteTver(QStringList dataList, tverStruct* tver);
    QString HexWriteDataStruct(QStringList dataList, dataStruct* struObject);


    void unsignedToSigned(uint32_t val, datTypDic typedic);
    QMap<uint32_t, QString> typeCode;
    QMap<uint32_t, QString> funcCode;
    QMap<uint32_t, QString> ackCode;

    QVector<tbs>* tbsUnion;
    bool ItemToTbs(QString text);
    void SetStatusToBox(Ui::Widget *ui);
    void SetStatusToGBox(QGridLayout *gridLayout);
    void SetStatusToLBox(QGridLayout *gridLayout, QList<QString> strL, QList<QLabel*> labelL, uint32_t val);

    bool CsvToTbs(QByteArray csvData);

    QString GetTime(QString csvData);
private:

};



#endif // TEXTDECODE_H


