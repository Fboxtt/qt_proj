#include "hexDecode.h"

hexDecode::hexDecode(void)
{

}
uint32_t hexDecode::toUInt(QByteArray inputArray)
{
    bool ok;
//    qDebug() << QString::fromLocal8Bit(inputArray);
    return QString::fromLocal8Bit(inputArray).toUInt(&ok, 16);
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
QString hexDecode::ReadHexFile(QFile *file)
{
    QByteArray lineData;
    bool ok = false;
    uint32_t lineNumber = 0;
    QString decodeLog = "";
    uint32_t lastAddr;
    // 如果解析正确，则exist
    this->exist = false;
    while(true) {
        lineNumber++;
        lineData = file->readLine();
        if(lineData == "") {
            break;
        }
        // 判断线数据【0】是否是“：“
        if(lineData[0] != ':') {
            decodeLog += QString(": 字符错误，缺失冒号--错误行号 = %1\n").arg(lineNumber);
            continue;
        }
        if(lineData.size() < COLON_L + DATAL_L + R_N_L) {
            qDebug() << "lenth < 5";
            decodeLog += QString(": 长度错误------------错误行号 = %1\n").arg(lineNumber);
            //长度错误，为避免内存越界，故读取下一行数据
            continue;
        }
        // datalenth
        QString lenthStr = QString::fromLocal8Bit(lineData.mid(1, 2));
        uint32_t dateLenth = lenthStr.toUInt(&ok, 16);

        // 计算实际长度和datalenth是否相等
        if((NOT_DATA_L + dateLenth * 2) != (uint32_t)(lineData.size())) {
            qDebug() << (NOT_DATA_L + dateLenth * 2) << (lineData.size());
            decodeLog += QString(": 长度错误------------错误行号 = %1\n").arg(lineNumber);
            //长度错误，为避免内存越界，故读取下一行数据
            continue;
        }

        // 获取.HEX行数据的基本信息
        uint32_t dataAddress = this->toUInt(lineData.mid(COLON_L + DATAL_L, 4));
        uint32_t dataType     = this->toUInt(lineData.mid(COLON_L + DATAL_L + ADDRESS_L, 2));
        uint8_t dataCheckSum = (uint8_t)dateLenth + this->toUInt(lineData.mid(COLON_L + DATAL_L, 2)) + this->toUInt(lineData.mid(COLON_L + DATAL_L + 2, 2)) + dataType;
        uint8_t dataCheck    = this->toUInt(lineData.mid(lineData.size() - 4, 2));

        if(dataType > 6) {
            decodeLog += QString(": 数据类型错误--------错误行号 = %1\n").arg(lineNumber);
            continue;
        } else if(dataType == 0) {
            if(this->n00dataArray.size() == 0) {
                this->address = dataAddress;
            } else {
                if(lastAddr != dataAddress) {
                    decodeLog += QString(": 地址错误------------错误行号 = %1\n").arg(lineNumber);
                    lastAddr = dataAddress + dateLenth;
                    continue;
                }
            }
            lastAddr = dataAddress + dateLenth;
        }

        // 把.HEX内的字符串数据，转换成HEX存入对象
        for(uint32_t i = 0; i < dateLenth; i++) {
            uint8_t byteNum = QString::fromLocal8Bit(lineData.mid(DATA_START + i * 2, 2)).toUInt(&ok, 16);
            dataCheckSum += (uint8_t)byteNum;

            switch(dataType) {
            case 0:
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
            }
        }

        // 检查校验和是否正确
        if(dataCheckSum != (uint8_t)(0x100 - dataCheck)) {
            decodeLog += QString(": 校验错误------------错误行号 = %1\n").arg(lineNumber);
        }

    }
    // 获取扩展线性地址 08 00 -> 0x0800, 获取程序起始地址
    this->extendLinearAddress = QString::fromLocal8Bit(this->n04extendLinearArray).toUInt(&ok, 16);
    this->address = this->address + (this->extendLinearAddress << 4);
    if(decodeLog == "") {
        decodeLog += ": 正确---------------------错误行号 = 无\n";
        this->exist = true;
    }
    return decodeLog;
}
void hexDecode::Clear(void)
{
    lenth = 0;
    address = 0;
    extendLinearAddress = 0;
    dataType = 0;
    n00dataArray.clear();
    n01endArray.clear();
    n02extendArray.clear();
    n03startArray.clear();
    n04extendLinearArray.clear();
    n05startLinearArray.clear();
}
