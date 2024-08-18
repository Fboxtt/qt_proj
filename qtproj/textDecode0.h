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

    bool newDataStatus;

    QList<QString> keyList;
    QList<dataCell> dataCellList;
    uint32_t dataLenth;

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
    textStruct(){
        byteLenth = 0;
    }
    textStruct(QString text);
    QString tim;
    QString text;
    sendState sendOrReceive;
    dataErr Err;
    char ACK;
    char cmd;
    bool cmdOk;
    bool checkSumOk;
    bool lenthOk;
    int byteLenth;
    QByteArray dataArray;

};


class textDcode
{

public:
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


