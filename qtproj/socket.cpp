#include <QJsonDocument>
#include <QJsonObject>
#include "socket.h"

#include "serial0.h"
#include "csv1.h"
#include "chart.h"
#include "hexDecode.h"

extern textDcode dcode0;
extern chartV* chartV0;
extern hexDecode hexFile;
//testProcess testObj0; // 测试程序，包括socket模块和json解析模块



// void testObject::serverSend(QString str)
// {
//     // server send
//     QByteArray sWriteMsg = str.toUtf8();
// //    ui->sendData->appendPlainText("[server] 发送数据:" + str);
//     TcpSocket->write(sWriteMsg);
// }

// void testObject::serverReceive(QByteArray hex)
// {
//     // server receive
//     QJsonDocument doc = QJsonDocument::fromJson(hex);
//     if(doc.isNull()) {
//         if(doc.isObject()) {
//             this->rJsonObj = doc.object();
//             qDebug() << "接受json数据，并且数据有效";
//         }
//     }
// //    doc.toBinaryData()
// //    testObj0.rJsonObj.
// }

void testObject::add(QString key, void* val)
{
    handleMap.insert(key, val);
    mapSize++;
}

void* testObject::pFunc(QString key)
{
    if(handleMap.contains(key)) {
        return handleMap.value(key);
    } else {
        return 0;
    }
}

void testObject::QByteToJson(QByteArray array)
{
    QJsonDocument doc = QJsonDocument::fromJson(array);
    if(doc.isNull()) {
        if(doc.isObject()) {
            this->rJsonObj = doc.object();
            qDebug() << "接受json数据，并且数据有效" << this->rJsonObj;
        }
    }
}

QByteArray testObject::jsonToQByte()
{
    QJsonDocument doc(this->sJsonObj);
    QByteArray byteArray = doc.toJson();
//    QString jsonString = QString::fromUtf8(byteArray);
    return byteArray;
}

void testObject::clear()
{
     this->comStatus = testObject::NO_START;
    this->status = avalible;
    this->testingKey = "";
    this->step = 0;
}

void testObject::testProcess(QString key)
{
    if(this->status != testObject::testing) {
        this->status = testObject::testing;
        this->comStatus = testObject::NO_START;
        this->step = 0;
        this->testingKey = key;
    }

    if(bar == nullptr) {
        bar = new QProgressDialog(); // 烧录滑动窗口
        bar->setMinimumWidth(400);
        bar->setMinimumDuration(0);
        bar->setLabelText("正在烧录");
        bar->setCancelButtonText("烧录取消");
        bar->setWindowTitle(("烧录进度"));
        bar->setRange(0, 100);
        bar->setValue(1);
        bar->setValue(100);
    }

    if(key == "握手中断测试") {
        this->shakeInterrruptTest();
    }
//    hexSendList.append(writeStr);
}

void testObject::shakeInterrruptTest()
{
    static uint32_t packetNum;
    writeStr = "";

    if(this->comStatus == testObject::RECEIVING) {
        switch (this->step) {
        case 3:
            if (dcode0.cmdAck == ERR_NO && dcode0.haveHex == true && dcode0.dataHex.at(0) == 0x00) {
                this->comStatus = testObject::SENDING;
            } else {
                this->comStatus = testObject::FAILED;
            }
            break;
        case 4:
            if (dcode0.cmdAck == ERR_NO && dcode0.haveHex == true && packetNum + 1 == dcode0.cmdPacketNum) {
                if(packetNum + 1 < hexFile.packetNum) {
                    packetNum++;
                    this->step--;
                }
                this->comStatus = testObject::SENDING;
            } else {
                this->comStatus = testObject::FAILED;
            }
            break;
        case 5:
            if (dcode0.cmdAck == ERR_ALL_CHECK && dcode0.haveHex == true) {
                this->comStatus = testObject::FAILED;
            } else {
                this->comStatus = testObject::SENDING;
            }
            break;
        default:
            if (dcode0.cmdAck == ERR_NO && dcode0.haveHex == true) {
                this->comStatus = testObject::SENDING;
            } else {
                this->comStatus = testObject::FAILED;
            }
            break;
        }
        if(this->comStatus == testObject::SENDING) {
            this->step++;
            dcode0.haveHex = false;
        } else if(this->comStatus == testObject::FAILED) {
            qDebug() << this->testingKey << "测试失败 第" << this->step << "步失败";
            this->status = tested;
            this->reportLog = this->testingKey + "测试失败" + QString::number(this->step) + "步失败";
            this->step = 0;
            return;
        }
    } else if(this->comStatus == testObject::NO_START) {
        this->comStatus = testObject::SENDING;
        this->step = 0;
    }

    switch (this->step) {
    case 0:
        packetNum = 0;
        writeStr = hexFile.packetToSendString(hexDecode::ENTER_BOOTMODE);
        break;
    case 1:
        writeStr = hexFile.packetToSendString(hexDecode::ENTER_BOOTMODE);
        break;
    case 2:
        writeStr = hexFile.packetToSendString(hexDecode::ENTER_BOOTMODE);
        break;
    case 3:
        writeStr = hexFile.packetToSendString(hexDecode::DOWNLOAD_BUFFER);
        break;
    case 4:
        writeStr = hexFile.packetToSendString(hexDecode::WRITE_FLASH, packetNum);
        break;
    case 5:
//        writeStr = hexFile.packetToSendString(hexDecode::REC_TOTAL_CHECKSUM);
        writeStr = "00 00 06 01 78 55 AA 00 00 00"; // 7E才是正确的
        break;
    case 6:
        this->step = 0;
        this->status = testObject::tested;
//        qDebug() << this->testingKey << "测试成功";
        this->reportLog = this->testingKey + "测试成功";
//        ui->sendData->appendPlainText();
        return;
    }

//    this->sendCmdListFunc();
    this->comStatus = testObject::RECEIVING;

}
