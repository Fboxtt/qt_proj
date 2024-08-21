#include "hexDecode.h"

extern QVector<QByteArray> byteSendList;

hexDecode::hexDecode(void)
{
    this->writeFlashCmd = 0x55;
}
uint32_t hexDecode::toUInt(QByteArray inputArray)
{
    bool ok;
//    qDebug() << QString::fromLocal8Bit(inputArray);
    return QString::fromLocal8Bit(inputArray).toUInt(&ok, 16);
}
uint32_t hexDecode::litBytetoUInt(QByteArray inputArray)
{
    uint32_t outPutInt = 0;
    for(int i = 0; i < inputArray.size(); i++) {
        outPutInt += inputArray[i] << (8 * i);
    }
    return outPutInt;
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
    uint32_t lastAddr = 0;
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
                this->hexLenth++;
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
void hexDecode::DownloadClear(void)
{
    address = 0;
    extendLinearAddress = 0;
    dataType = 0;
    eraseFlag = 0;
    beginDownloadState = 0;
    packetNum = 0;
    shakeSuccessTime = 0;
    writeSuccessTime = 0;
    hexPacketoK.clear();
}
void hexDecode::AllClear(void)
{
    this->DownloadClear();
    this->hexLenth = 0;
    n00dataArray.clear();
    n01endArray.clear();
    n02extendArray.clear();
    n03startArray.clear();
    n04extendLinearArray.clear();
    n05startLinearArray.clear();

    hexArray.clear();
}

#define NO_DATA_TYPE_LENTH 4
QString hexDecode::packetToSendString(bmsCmdType cmdType, uint32_t packetNumber)
{
    QByteArray sendDataArray;
    QByteArray dataArray = {};
    if(cmdType == this->WRITE_FLASH) {
        dataArray.append((uint8_t)packetNumber);
        dataArray.append((uint8_t)(packetNumber >> 8));
        dataArray.append(this->n00dataArray.mid(packetNumber * this->packetSize, this->packetSize));
    }
    uint16_t dataArrayLenth = (NO_DATA_TYPE_LENTH + dataArray.size());
    sendDataArray.append(char(0x00));
    sendDataArray.append((uint8_t)(dataArrayLenth >> 8));
    sendDataArray.append((uint8_t)(dataArrayLenth));
    sendDataArray.append(char(0x01)); // 单板类型
    sendDataArray.append(char(cmdType)); //功能码
    sendDataArray.append(char(0x55));
    sendDataArray.append(char(0xAA));
//    sendDataArray.append(char(0x00)); //应答码
    sendDataArray.append(dataArray);
    uint16_t checkSum =0;
    checkSum += (uint16_t)(dataArrayLenth >> 8);
    checkSum += (uint16_t)(dataArrayLenth);
    checkSum += 0x01;
    checkSum += uint16_t(cmdType);
    checkSum += 0x55;
    checkSum +=  0xAA;
//    (((NO_DATA_TYPE_LENTH + dataArray.size()) & 0xff00) >> 8) + ((NO_DATA_TYPE_LENTH + dataArray.size()) & 0xff) + 0x01 + char(this->writeFlashCmd) + 0x55 + 0xAA + 0x00;
    foreach(uint8_t byte, dataArray) {
        checkSum += byte;
    }
    sendDataArray.append((uint8_t)checkSum);
    //    qDebug() << dataArray << dataArray.size();
    QString sendData = sendDataArray.toHex().data(),utf8Buffer;
    //    qDebug() << "sendDataArray size = " << sendDataArray.size();
    for(int i=0;i<sendDataArray.length();i++)
    {
           QString str_1 = sendData.mid (i * 2, 2);
           utf8Buffer += str_1;
           utf8Buffer += " ";
    }
    return utf8Buffer;
}

bool hexDecode::isDownLoadCmd(char cmd)
{
    cmd &= 0x7f;
    if(cmd == hexDecode::READ_BOOT_CODE_INF || \
            cmd == hexDecode::READ_IC_INF || \
            cmd == hexDecode::HEX_INFO || \
            cmd == hexDecode::EARSE_ALL || \
            cmd == hexDecode::ENTER_BOOTMODE || \
            cmd == hexDecode::WRITE_FLASH || \
            cmd == hexDecode::READ_FLASH) {
        return true;
    } else {
        return false;
    }
}

bool hexDecode::DownLoadProcess(textStruct text, QString* outPutStr)
{
    QByteArray outPutArray;
    bool enterWriteFlash = false;
    if((text.cmd & 0x80) == 0) {
        return false;
    }
    if(this->beginDownloadState != true) {
        return false;
    }
    if(text.cmd == (hexDecode::ENTER_BOOTMODE | 0x80)) {
        if(text.ACK == textStruct::ACK_OK) {
            this->shakeSuccessTime++;
        }
    }

    if(shakeSuccessTime < SHAKE_TIME_LIMIT) {
        *outPutStr = this->packetToSendString(this->ENTER_BOOTMODE, this->packetNum);
        return true;
    }
    if(text.cmd == (hexDecode::EARSE_ALL | 0x80)) {
        if(text.ACK == textStruct::ACK_OK) {
            eraseFlag = 1;
        }
    }
    if(eraseFlag == 0) {
        *outPutStr = this->packetToSendString(this->EARSE_ALL, this->packetNum);
        return true;
    }

    if((text.cmd == (hexDecode::EARSE_ALL| 0x80)) && eraseFlag == 1) {
        enterWriteFlash = true;
    } else if (text.cmd == (hexDecode::WRITE_FLASH | 0x80)) {
        if(text.ACK == textStruct::ACK_OK) {
            if(text.dataArray.size() == 2) {
                if(this->litBytetoUInt(text.dataArray) == (this->packetNum)) {
                    writeSuccessTime++;
                    this->packetNum++;
                    enterWriteFlash = true;
                    hexPacketoK.append(true);
                }
                if(this->packetNum > this->hexLenth) {
                    return false;
                }
            }
        }
        enterWriteFlash = true;
    }

    if(enterWriteFlash == true) {
        *outPutStr = this->packetToSendString(this->WRITE_FLASH, this->packetNum);
        return true;
    }
    *outPutStr = "";
    return false;
}
