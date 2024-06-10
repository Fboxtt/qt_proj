#include <textDecode0.h>



datTypDic::datTypDic(DATA_TYPE type, QString typeName, uint32_t typeLenth, ENDIAN_TYPE endianType, SIGNED_TYPE signedType)
{
    this->type = type;
    this->typeName = typeName;
    this->typeLenth = typeLenth;
    this->endianType = endianType;
    this->signedType = signedType;
}

QList<datTypDic> TypUnion = {
    {datTypDic::ULONG,   "ULONG", 4, datTypDic::LITTLE, datTypDic::UNSIGNED},
    {datTypDic::LONG,     "LONG", 4, datTypDic::LITTLE, datTypDic::SIGNED},
    {datTypDic::USHORT, "USHORT", 2, datTypDic::LITTLE, datTypDic::UNSIGNED},
    {datTypDic::SHORT,   "SHORT", 2, datTypDic::LITTLE, datTypDic::SIGNED},
    {datTypDic::CHAR,     "CHAR", 1, datTypDic::LITTLE, datTypDic::SIGNED},
};
uint32_t tbs::datInCmdAddr = 8;
uint32_t tbs::datLenth = 0;
tbs::tbs(QString valName,datTypDic::DATA_TYPE dataType)
{
    this->valName = valName;
    this->unitInCmdAddr = tbs::datInCmdAddr + tbs::datLenth;
    this->unitInDatAddr = tbs::datLenth;
    this->dataType = dataType;

    foreach(datTypDic unit, TypUnion) {
        if(unit.type == this->dataType) {
            this->typeLenth = unit.typeLenth;
            this->datLenth += unit.typeLenth;
            this->endianType = unit.endianType;
            this->signedType = unit.signedType;
        }
    }
}

//datTypDic tbs::getType(void)
//{
//    foreach(datTypDic unit, TypUnion) {
//        if(unit.type == this->dataType) {
//            return unit;
//        }
//    }
//    qDebug() << "warning :no this dataType";
//    return TypUnion[0];
//}

QVector<tbs> tbsUnit = {
    {"端口电压mV", datTypDic::ULONG}, \
    {"电池电压mV", datTypDic::ULONG}, \
    {"0电芯电压mV", datTypDic::USHORT}, \
    {"1电芯电压mV", datTypDic::USHORT}, \
    {"2电芯电压mV", datTypDic::USHORT}, \
    {"3电芯电压mV", datTypDic::USHORT}, \
    {"4电芯电压mV", datTypDic::USHORT}, \
    {"5电芯电压mV", datTypDic::USHORT}, \
    {"6电芯电压mV", datTypDic::USHORT}, \
    {"7电芯电压mV", datTypDic::USHORT}, \
    {"8电芯电压mV", datTypDic::USHORT}, \
    {"9电芯电压mV", datTypDic::USHORT}, \
    {"10电芯电压mV", datTypDic::USHORT}, \
    {"11电芯电压mV", datTypDic::USHORT}, \
    {"12电芯电压mV", datTypDic::USHORT}, \
    {"13电芯电压mV", datTypDic::USHORT}, \
    {"14电芯电压mV", datTypDic::USHORT}, \
    {"15电芯电压mV", datTypDic::USHORT}, \
    {"电流值mA", datTypDic::LONG}, \
    {"0温度值℃", datTypDic::SHORT}, \
    {"1温度值℃", datTypDic::SHORT}, \
    {"2温度值℃", datTypDic::SHORT}, \
    {"3温度值℃", datTypDic::SHORT}, \
    {"4温度值℃", datTypDic::SHORT}, \
    {"usRmAH", datTypDic::USHORT}, \
    {"usFccAH", datTypDic::USHORT}, \
    {"usBiaAH", datTypDic::USHORT}, \
    {"otherInfo", datTypDic::ULONG}, \
    {"alarmStatus", datTypDic::ULONG}, \
    {"protectStatus", datTypDic::ULONG}, \
    {"faultStatus", datTypDic::ULONG}, \
    {"balanceStatus", datTypDic::ULONG}, \

    {"BattStatus", datTypDic::USHORT}, \

    {"SOCPct%", datTypDic::USHORT}, \

    {"SOHPct%", datTypDic::ULONG}, \
    {"DisTimes次", datTypDic::ULONG}, \
    {"TotalDisAH", datTypDic::ULONG}, \
};
//tbs tbsUnit(nullptr,0,0+4);

textDcode::textDcode(void)
{
    funcCode =  {{0x01,"产品注册"} \
                ,{0x02,"断开注册"}\
                ,{0x13,"bat状态"}\
                ,{0x0A,"开chg fet"}\
                ,{0x0B,"关chg fet"}\
                ,{0x0C,"开dchg fet"}\
                ,{0x0D,"关dchg fet"}\
                ,{0x10,"获取产品序列号"}\
                ,{0x16,"获取版本"}\
                ,{0x60,"关机命令"}\
                ,{0x64,"休眠命令"}};

    ackCode =   {{0x00,"ACK无异常"}\
                ,{0x01,"长度错误"}\
                ,{0x03,"类型错误"}\
                ,{0x04,"id错误"}\
                ,{0x05,"握手错误"}\
                ,{0x06,"校验错误"}};



}

QString textDcode::ByteDecode(QMap<uint32_t, QString> mapCode, uint8_t keys)
{
    QString strCode;
    if (!mapCode.contains(keys)) {
        strCode =  "--";
        return strCode;
    }
    strCode = mapCode.value(keys);
    return strCode;
}



uint32_t textDcode::TbsDecode(QVector<uint8_t> hexVector)
{
    uint32_t checkSum = 0;
    foreach(uint8_t hexVal, hexVector) {
        checkSum += hexVal;
    }
    return checkSum;
}

QString textDcode::readDataDocode(QStringList hexStrLis, QString decodeStr)
{
    QStringList Command = {"地址","高字节","低字节","单板类型","功能码","握手1","握手2","ack","data","check"};
    QVector<uint8_t> hexVector;
    uint32_t dataNum = 0, dataSum = 0, datId = 0;
    decodeStr = "";
    bool ok;

    foreach(QString hexStr, hexStrLis) {
        hexVector.append((uint8_t)hexStr.toInt(&ok, 16));
    }

    for (uint32_t i = 0, limit = Command.size(); i < limit; i++) {
        qDebug() << i <<"decodeStr = " << hexStrLis[datId] << decodeStr;
        if (Command[i] == "地址") {
            decodeStr.append(hexStrLis[datId] + " ");
        } else if (Command[i] == "高字节") {
            decodeStr.append(hexStrLis[datId] + " ");
            dataNum += hexVector[datId] * 256;
            dataSum += hexVector[datId];
        } else if (Command[i] == "低字节") {
            decodeStr.append(hexStrLis[datId] + " ");
            dataNum += hexVector[datId];
            dataSum += hexVector[datId];
        } else if (Command[i] == "功能码") {
            decodeStr.append(this->ByteDecode(this->funcCode, (hexVector[datId]) - 0x80) + " ");
            dataSum += hexVector[datId];
        } else if (Command[i] == "ack") {
            decodeStr.append(this->ByteDecode(this->ackCode, hexVector[datId]) + " ");
            dataSum += hexVector[datId];
        } else if (Command[i] == "data") {
            if (dataNum == 5) {
            } else if (dataNum > 5) {
                dataSum += this->TbsDecode(hexVector.mid(i,dataNum - 5));
                decodeStr.append(hexStrLis.mid(datId, dataNum - 5).join(" ") + " ");
                datId += dataNum - 5;
            }
            continue;
        } else if (Command[i] == "check") {
            qDebug() << "dataSum = " << dataSum << "& 0xff = " << (dataSum & 0xff);
            if ((dataSum & 0xff) == hexVector[datId]) {
                decodeStr.append("校验正确");
            } else {
                decodeStr.append("校验错误");
            }
        } else {
            decodeStr.append(hexStrLis[datId] + " ");
            dataSum += hexVector[datId];
        }
        datId++;
    }
    qDebug() << "endstr:" << decodeStr;
    return decodeStr;
}
QString textDcode::SendCmdDocode(QStringList hexStrLis, QString decodeStr)
{
    QString Command[8] = {"地址","低字节","高字节","单板类型","功能码","握手1","握手2","check"};
    QString strm;
    decodeStr = "";
    bool ok;
    for (uint32_t i = 0, limit = hexStrLis.size(); i < limit; i++) {
        qDebug() << hexStrLis[i].toInt(&ok, 16) << decodeStr;

        if (Command[i] == "功能码") {
            decodeStr.append(this->ByteDecode(this->funcCode, hexStrLis[i].toInt(&ok, 16)) + " ");
        } else if (Command[i] == "ack") {
            decodeStr.append(this->ByteDecode(this->ackCode, hexStrLis[i].toInt(&ok, 16)) + " ");
        } else {
            decodeStr.append(hexStrLis[i] + " ");
        }
    }
    qDebug() << "endstr:" << decodeStr;
    return decodeStr;
}

QString textDcode::SendDataDecode(Ui::Widget *ui, QString decodeStr)
{
    QString outToPlainText;
//    Data += '\r';//插入换行
    qDebug() << ui->TimeCheckBox->isChecked() << "time checkbox";
    if (ui->TimeCheckBox->isChecked()) {
        outToPlainText = QString("[%1]:TX&->").arg(QTime::currentTime().toString("HH:mm:ss:zzz")) + decodeStr;
    }
    ui->receiveData->appendPlainText(outToPlainText);

    return this->DecodeHexToCommand(ui);
}

QString textDcode::DecodeHexToCommand(Ui::Widget *ui)
{
    QString Command[10] = {"地址","低字节","高字节","单板类型","功能码","握手1","握手2","ack","data","check"};
    QString timeText, dataText;
    QTextBlock textBlock;
    QTextDocument *doc;
    QByteArray textByte;
    QStringList timeAndDataList, dataList;
    int blockCount;

//    Command[0] = "address";
//    ui->receiveData->setPlainText("00 00 05 01 81 55 AA 00");
//    ui->receiveData->setPlainText("00 00 04 01 01 55 AA 06");
//    ui->HexEncodeText->appendPlainText(ui->receiveData->);
    qDebug() << ui->receiveData->blockCount() << "block count";

    doc = ui->receiveData->document();

    blockCount = ui->receiveData->blockCount();

    textBlock = doc->findBlockByNumber(blockCount - 1); // 获得末尾的text

    dataText = textBlock.text();
    timeText = "";

    if (dataText.contains("->", Qt::CaseSensitive)) {
        timeAndDataList = dataText.split("->"); // 和时间戳分开
        timeText = timeAndDataList[0];
        dataText = timeAndDataList[1];
    }

    if (dataText.contains(QRegExp("^[0-9a-fA-F]{1,}$")) == true) {
        dataText = "数据非法";
        return dataText;
    }
    qDebug() << textBlock.text() << "text block";

    dataList = dataText.simplified().split(' ');

    qDebug() << dataList << "dataList out<<" << dataList.size()<<" list size" ;
    // 通过长度判断是send还是receive

    if (dataList.size() == 8) {
        dataText = SendCmdDocode(dataList, dataText);
        qDebug() << "Decode=8 endstr" << dataText;
        // send data
    } else if (dataList.size() > 8 && dataList.size() < 200) {
        dataText = readDataDocode(dataList, dataText);
        qDebug() << "Decode>8 endstr" << dataText;
        // receive data
    } else {
        qDebug() << "无法解析";
        dataText = "无法解析";
        // error
    }

    //单独解码

    //返回并显示
    return timeText + "->" + dataText;

}



void textDcode::itemToTable(Ui::Widget *ui, QVector<tbs> dataList, QVector<QTableWidgetItem>* itemTableList)
{
    for(int idx = 0, limit = itemTableList->size(); idx < limit; idx++) {
        (*itemTableList)[idx].setText("");
    }
    uint32_t idx = 0;
    foreach(tbs unit, dataList) {
        (*itemTableList)[idx].setText(unit.valName + " = " + QString("%1").arg(unit.uintVal,0,10));
        qDebug() << "idx / 3 = " << idx / 4 << "% 3 = " << idx % 4;
//        ui->tableWidget->setItem(idx / 3, idx % 3, &itemTableList[idx]);
        idx++;
    }
}

void textDcode::unsignedToSigned(uint32_t val, datTypDic typedic)
{

}
QVector<tbs> textDcode::HexToStr(Ui::Widget *ui, QStringList dataList)
{

    bool ok;
    QVector<uint8_t> hexVector;
    uint8_t byteInData = 0;
    uint32_t tbsUnitIdx = 0, uintVal = 0;

    foreach(QString hexStr, dataList) {
        hexStr.toInt(&ok, 16);
        if (ok == true) {
            hexVector.append((uint8_t)hexStr.toInt(&ok, 16));
        }
    }
    qDebug() << "hexvector = " << hexVector << "size = " << hexVector.size();
    foreach(uint8_t hex, hexVector) {
        if (byteInData == tbsUnit[tbsUnitIdx].typeLenth) {
//            this->unsignedToSigned(uintVal, TypUnion[tbsUnit[tbsUnitIdx]]);
            tbsUnit[tbsUnitIdx].uintVal = uintVal;
            qDebug() << tbsUnit[tbsUnitIdx].valName <<"uintval%d = " << uintVal;

            tbsUnitIdx++;
            uintVal = 0;
            byteInData = 0;
        }
//        qDebug() << "hex = " << hex;
        uintVal += (((uint32_t)hex) <<  byteInData * 8);
        byteInData++;
    }
    return tbsUnit;

}
