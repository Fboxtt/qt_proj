#include "hexDecode.h"

hexDecode::hexDecode(void)
{

}

bool hexDecode::OpenHexFile(QFile *pfile, QString fileUrl)
{
    pfile->setFileName(fileUrl);
    if(!pfile->open(QIODevice::ReadOnly))
    {
        qDebug()<<"文件打开失败";
        pfile->close();
        return false;
    }

    return true;
}
#define COLON_L 1
#define DATAL_L 2
#define ADDRESS_L 4
#define TYPE_L 2
#define CHECK_L 2
#define R_N_L 2
#define DATA_START (COLON_L + DATAL_L + ADDRESS_L + TYPE_L - 1)
#define NOT_DATA_L (COLON_L + DATAL_L + ADDRESS_L + TYPE_L + CHECK_L + R_N_L)
void hexDecode::ReadHexFile(QFile *file)
{
    QByteArray lineData;
    while(true) {
        lineData = file->readLine();
        if(lineData == "") {
            break;
        }
        qDebug() << "hex data" << lineData << "len = " << lineData.size();
        if(lineData[0] != ':') {
            qDebug() << "hex data err , no exist \":\"";
        }
        if(lineData.size() < COLON_L + DATAL_L + R_N_L) {
            qDebug() << "lenth < 5";
            break;
        }
        QString lenthStr = QString::fromLocal8Bit(lineData.mid(1, 2));

        qDebug() << lenthStr;

        bool ok = false;

        uint32_t dateLenth = lenthStr.toUInt(&ok, 16);

        if((NOT_DATA_L + dateLenth * 2) != (lineData.size())) {
            qDebug() << (NOT_DATA_L + dateLenth * 2) << (lineData.size());

            break;
        }

        uint32_t dataAddress = QString::fromLocal8Bit(lineData.mid(3, 4)).toUInt(&ok, 16);

        uint32_t dataType = QString::fromLocal8Bit(lineData.mid(7, 2)).toUInt(&ok, 16);

        uint32_t dataCheck = QString::fromLocal8Bit(lineData.mid(lineData.size() - 4, 2)).toUInt(&ok, 16);

        for(uint32_t i = 0; i < dateLenth; i++) {
            uint8_t byteNum = QString::fromLocal8Bit(lineData.mid(DATA_START + i * 2, 2)).toUInt(&ok, 16);
            this->hexArray.append(byteNum);
            this->dataArray.append(byteNum);
            this->lenth++;
        }

    }
//    qDebug() << "hexArray" << this->hexArray;
    qDebug() << "dataArray" << dataArray;
    qDebug() << "lenth" << lenth << "dataarraylenth" << dataArray.size();
}
