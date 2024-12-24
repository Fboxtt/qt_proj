#include <QJsonDocument>
#include <QJsonObject>
#include "socket.h"

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

