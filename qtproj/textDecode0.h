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

class tbs
{
    static uint32_t datInCmdAddr;
    static uint32_t datLenth;

public:
    tbs(QString valName, datTypDic::DATA_TYPE dataType);
    tbs(){};
//    datTypDic getType(void);
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
    QVector<tbs> IntWriteTbs(QStringList dataList);
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
private:

};



#endif // TEXTDECODE_H


