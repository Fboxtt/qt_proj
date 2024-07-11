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
#define DATA_START (COLON_L + DATAL_L + ADDRESS_L + TYPE_L)
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
        bool ok = false;
        QString lenthStr = QString::fromLocal8Bit(lineData.mid(1, 2));
        uint32_t dateLenth = lenthStr.toUInt(&ok, 16);
        qDebug() << lenthStr << "line data lenth" << dateLenth;





        if((NOT_DATA_L + dateLenth * 2) != (lineData.size())) {
            qDebug() << (NOT_DATA_L + dateLenth * 2) << (lineData.size());

            break;
        }

        uint32_t dataAddress = QString::fromLocal8Bit(lineData.mid(COLON_L + DATAL_L, 4)).toUInt(&ok, 16);

        uint32_t dataType     = QString::fromLocal8Bit(lineData.mid(COLON_L + DATAL_L + ADDRESS_L, 2)).toUInt(&ok, 16);

        uint8_t dataCheck    = QString::fromLocal8Bit(lineData.mid(lineData.size() - 4, 2)).toUInt(&ok, 16);

        uint8_t dataCheckSum = (uint8_t)dateLenth + (uint8_t)dataAddress + dataType;
        qDebug() << dataType << "dataType";
//        QByteArray data;
        for(uint32_t i = 0; i < dateLenth; i++) {
            uint8_t byteNum = QString::fromLocal8Bit(lineData.mid(DATA_START + i * 2, 2)).toUInt(&ok, 16);
            qDebug() << "byteNum" << byteNum <<  lineData.mid(DATA_START + i * 2, 2);
            dataCheckSum += (uint8_t)byteNum;
            switch(dataType) {
            case 0:
                this->hexArray.append(byteNum);
                this->n00dataArray.append(byteNum);
                this->lenth++;
                break;
            case 1:
                this->n01endArray.append(byteNum);
                break;
            case 2:
                this->n02extendArray.append(byteNum);
                break;
            case 3:
                this->n03startArray.append(byteNum);
                break;
            case 4:
                this->n04extendLinearArray.append(byteNum);
                break;
            case 5:
                this->n05startLinearArray.append(byteNum);
                break;
            default:
                qDebug() << "decode .hex : no this datatype";
            }
        }



        if((uint8_t)((~dataCheckSum)+ 1) != dataCheck) {
            qDebug() << "decode .hex : checkSum err" << (uint8_t)((~dataCheckSum)+1) << dataCheck;
        } else {
            qDebug() << "decode .hex : checkSum right";
        }
    }
    // 获取扩展线性地址 08 00 -> 0x0800
    qDebug() << "size extendarray" << this->n04extendLinearArray.length();
    this->extendLinearAddress = (uint8_t)(this->n04extendLinearArray.at(0)) * 256 + (uint8_t)this->n04extendLinearArray.at(1);
//    qDebug() << "dataArray" << n00dataArray;
    qDebug() << "lenth" << lenth << "dataArraylenth" << n00dataArray.size();
}
