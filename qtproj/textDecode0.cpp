#include <textDecode0.h>

extern tverStruct *tverStru0;
extern caliStruct *caliStru0;
extern tbsStruct *tbsStru0;
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
    {datTypDic::UCHAR,     "UCHAR", 1, datTypDic::LITTLE, datTypDic::SIGNED},
    {datTypDic::STRING,     "STRING", 1, datTypDic::LITTLE, datTypDic::UNSIGNED},
};
// ***************************************dataCell**************************************//
// ***************************************dataCell**************************************//
dataCell::dataCell(QString valName, datTypDic::DATA_TYPE dataType, uint32_t lenth)
{
    this->valName = valName;
//    this->unitInCmdAddr = tver::datInCmdAddr;
    // this->unitInDatAddr = tver::datLenth;
    this->dataType = dataType;

    foreach(datTypDic unit, TypUnion) {
        if(unit.type == this->dataType) {
            this->typeLenth = lenth;
            // this->datLenth += lenth;
            this->endianType = unit.endianType;
            this->signedType = unit.signedType;
        }
    }
}
dataCell::dataCell(QString valName, datTypDic::DATA_TYPE dataType)
{
    this->valName = valName;
    // this->unitInCmdAddr = tver::datInCmdAddr + tver::datLenth;
    // this->unitInDatAddr = tver::datLenth;
    this->dataType = dataType;

    foreach(datTypDic unit, TypUnion) {
        if(unit.type == this->dataType) {
            this->typeLenth = unit.typeLenth;
            // this->datLenth += unit.typeLenth;
            this->endianType = unit.endianType;
            this->signedType = unit.signedType;
        }
    }
}
// ***************************************dataStruct**************************************//
// ***************************************dataStruct**************************************//
dataStruct::dataStruct()
{
    this->newDataStatus = false;
    this->dataLenth = 0;
}

dataCell* dataStruct::value(QString valName)
{
    uint32_t i = 0;
    foreach(QString key, keyList) {
        if(key == valName) {
            return &dataCellList[i];
        }
        i++;
    }
    qDebug() << "没有这个key = " << valName;
    return &dataCellList[0];
}

void dataStruct::insert(dataCell addCell)
{
    keyList.append(addCell.valName);
    addCell.uintVal = 0;
    this->dataCellList.append(addCell);
    this->dataLenth += addCell.typeLenth;
}
// ***************************************cliStruct**************************************//
// ***************************************cliStruct**************************************//
caliStruct::caliStruct()
{
    this->insert({"usPackVk", datTypDic::USHORT});
    this->insert({"usBattVk", datTypDic::USHORT});

    for(uint8_t i = 1; i <= 16; i++)
    {
        this->insert({"电芯" + QString::number(i,10), datTypDic::USHORT});//
    }

    this->insert({"usChgCurrK", datTypDic::USHORT});
    this->insert({"sChgCurrB", datTypDic::SHORT});
    this->insert({"usDisCurrK", datTypDic::USHORT});
    this->insert({"sDisCurrB", datTypDic::SHORT});
    this->insert({"usChgCurrSK", datTypDic::USHORT});
    this->insert({"sChgCurrSB", datTypDic::SHORT});
    this->insert({"usDisCurrSK", datTypDic::USHORT});
    this->insert({"sDisCurrSB", datTypDic::SHORT});
    this->insert({"usChgCurrSSK", datTypDic::USHORT});
    this->insert({"sChgCurrSSB;", datTypDic::SHORT});
    this->insert({"usDisCurrSSK", datTypDic::USHORT});
    this->insert({"sDisCurrSSB", datTypDic::SHORT});
    for(uint8_t i = 1; i <= 8; i++)
    {
        this->insert({"温度" + QString::number(i,10), datTypDic::USHORT});//
    }

    qDebug() << this->value("usChgCurrK")->valName << this->dataLenth;
}
// ***************************************tbsStruct**************************************//
// ***************************************tbsStruct**************************************//
tbsStruct::tbsStruct()
{
    this->dataLenth = 0;
    this->newDataStatus = false;

       this->insert({"PACK电压mV", datTypDic::ULONG});
       this->insert({"电池电压mV", datTypDic::ULONG});
       this->insert({"1电芯电压mV", datTypDic::USHORT});
       this->insert({"2电芯电压mV", datTypDic::USHORT});
       this->insert({"3电芯电压mV", datTypDic::USHORT});
       this->insert({"4电芯电压mV", datTypDic::USHORT});
       this->insert({"5电芯电压mV", datTypDic::USHORT});
       this->insert({"6电芯电压mV", datTypDic::USHORT});
       this->insert({"7电芯电压mV", datTypDic::USHORT});
       this->insert({"8电芯电压mV", datTypDic::USHORT});
       this->insert({"9电芯电压mV", datTypDic::USHORT});
       this->insert({"10电芯电压mV", datTypDic::USHORT});
       this->insert({"11电芯电压mV", datTypDic::USHORT});
       this->insert({"12电芯电压mV", datTypDic::USHORT});
       this->insert({"13电芯电压mV", datTypDic::USHORT});
       this->insert({"14电芯电压mV", datTypDic::USHORT});
       this->insert({"15电芯电压mV", datTypDic::USHORT});
       this->insert({"16电芯电压mV", datTypDic::USHORT});
       this->insert({"电流值mA", datTypDic::LONG});
       this->insert({"1温度值℃", datTypDic::SHORT});
       this->insert({"2温度值℃", datTypDic::SHORT});
       this->insert({"3温度值℃", datTypDic::SHORT});
       this->insert({"4温度值℃", datTypDic::SHORT});
       this->insert({"5温度值℃", datTypDic::SHORT});
       this->insert({"剩余容量AH", datTypDic::USHORT});
       this->insert({"满充容量AH", datTypDic::USHORT});
       this->insert({"显示和真实容量差", datTypDic::USHORT});
       this->insert({"其他信息HEX", datTypDic::ULONG});
       this->insert({"告警状态HEX", datTypDic::ULONG});
       this->insert({"保护状态HEX", datTypDic::ULONG});
       this->insert({"错误状态HEX", datTypDic::ULONG});
       this->insert({"均衡状态HEX", datTypDic::ULONG});

       this->insert({"电池状态HEX", datTypDic::USHORT});
       this->insert({"剩余容量%", datTypDic::USHORT});
       this->insert({"电池健康%", datTypDic::ULONG});
       this->insert({"放电次数", datTypDic::ULONG});
       this->insert({"总充电次数", datTypDic::ULONG});
}
// ***************************************tver**************************************//
tver::tver(QString valName, datTypDic::DATA_TYPE dataType, uint32_t lenth)
{
    this->valName = valName;
//    this->unitInCmdAddr = tver::datInCmdAddr;
    // this->unitInDatAddr = tver::datLenth;
    this->dataType = dataType;

    foreach(datTypDic unit, TypUnion) {
        if(unit.type == this->dataType) {
            this->typeLenth = lenth;
            // this->datLenth += lenth;
            this->endianType = unit.endianType;
            this->signedType = unit.signedType;
        }
    }
}
tver::tver(QString valName, datTypDic::DATA_TYPE dataType)
{
    this->valName = valName;
    // this->unitInCmdAddr = tver::datInCmdAddr + tver::datLenth;
    // this->unitInDatAddr = tver::datLenth;
    this->dataType = dataType;

    foreach(datTypDic unit, TypUnion) {
        if(unit.type == this->dataType) {
            this->typeLenth = unit.typeLenth;
            // this->datLenth += unit.typeLenth;
            this->endianType = unit.endianType;
            this->signedType = unit.signedType;
        }
    }
}

tverStruct::tverStruct()
{
    this->dataLenth = 0;
    this->newDataStatus = false;
    this->insert({"主版本号", datTypDic::USHORT});
    this->insert({"次版本号", datTypDic::USHORT});
    this->insert({"修订版本", datTypDic::USHORT});
    this->insert({"编译年", datTypDic::USHORT});
    this->insert({"编译月", datTypDic::UCHAR});
    this->insert({"编译日", datTypDic::UCHAR});
    this->insert({"硬件版本", datTypDic::STRING, 30});
    this->insert({"功能版本", datTypDic::STRING, 40});

    qDebug() << this->value("主版本号").valName << this->dataLenth;
};
tver tverStruct::value(QString valName)
{
    return tverMap.value(valName);
}

void tverStruct::insert(tver addTver)
{
    keyList.append(addTver.valName);
    tverMap.insert(addTver.valName, addTver);
    this->dataLenth += addTver.typeLenth;
}


tbs::tbs(QString valName,datTypDic::DATA_TYPE dataType)
{
    this->valName = valName;
    this->dataType = dataType;

    foreach(datTypDic unit, TypUnion) {
        if(unit.type == this->dataType) {
            this->typeLenth = unit.typeLenth;
//            this->datLenth += unit.typeLenth;
            this->endianType = unit.endianType;
            this->signedType = unit.signedType;
        }
    }
}

// QVector<tbs> tbsUnit = {
//     {"PACK电压mV", datTypDic::ULONG}, \
//     {"电池电压mV", datTypDic::ULONG}, \
//     {"1电芯电压mV", datTypDic::USHORT}, \
//     {"2电芯电压mV", datTypDic::USHORT}, \
//     {"3电芯电压mV", datTypDic::USHORT}, \
//     {"4电芯电压mV", datTypDic::USHORT}, \
//     {"5电芯电压mV", datTypDic::USHORT}, \
//     {"6电芯电压mV", datTypDic::USHORT}, \
//     {"7电芯电压mV", datTypDic::USHORT}, \
//     {"8电芯电压mV", datTypDic::USHORT}, \
//     {"9电芯电压mV", datTypDic::USHORT}, \
//     {"10电芯电压mV", datTypDic::USHORT}, \
//     {"11电芯电压mV", datTypDic::USHORT}, \
//     {"12电芯电压mV", datTypDic::USHORT}, \
//     {"13电芯电压mV", datTypDic::USHORT}, \
//     {"14电芯电压mV", datTypDic::USHORT}, \
//     {"15电芯电压mV", datTypDic::USHORT}, \
//     {"16电芯电压mV", datTypDic::USHORT}, \
//     {"电流值mA", datTypDic::LONG}, \
//     {"1温度值℃", datTypDic::SHORT}, \
//     {"2温度值℃", datTypDic::SHORT}, \
//     {"3温度值℃", datTypDic::SHORT}, \
//     {"4温度值℃", datTypDic::SHORT}, \
//     {"5温度值℃", datTypDic::SHORT}, \
//     {"剩余容量AH", datTypDic::USHORT}, \
//     {"满充容量AH", datTypDic::USHORT}, \
//     {"显示和真实容量差", datTypDic::USHORT}, \
//     {"其他信息HEX", datTypDic::ULONG}, \
//     {"告警状态HEX", datTypDic::ULONG}, \
//     {"保护状态HEX", datTypDic::ULONG}, \
//     {"错误状态HEX", datTypDic::ULONG}, \
//     {"均衡状态HEX", datTypDic::ULONG}, \

//     {"电池状态HEX", datTypDic::USHORT}, \
//     {"剩余容量%", datTypDic::USHORT}, \
//     {"电池健康%", datTypDic::ULONG}, \
//     {"放电次数", datTypDic::ULONG}, \
//     {"总充电次数", datTypDic::ULONG}, \
// };

textDcode::textDcode(void)
{
    funcCode =  {{0x01,"产品注册"} \
                ,{0x02,"断开注册"}\
                ,{0x13,"TBS数据"}\
                ,{0x0A,"开chg fet"}\
                ,{0x0B,"关chg fet"}\
                ,{0x0C,"开dchg fet"}\
                ,{0x0D,"关dchg fet"}\
                ,{0x10,"获取产品序列号"}\
                ,{0x16,"获取版本"}\
                ,{0x60,"关机命令"}\
                ,{0x64,"休眠命令"}};

    ackCode =   {{0x00,"ACK无异常"}\
                ,{0x01,"ACK长度错误"}\
                ,{0x03,"ACK类型错误"}\
                ,{0x04,"ACKid错误"}\
                ,{0x05,"ACK握手错误"}\
                ,{0x06,"ACK校验错误"}};

//    this->tbsUnion = &tbsUnit;
}

QList<QLabel*> alarmLabel;
QList<QLabel*> loseLabel;
QList<QLabel*> otherLabel;
QList<QLabel*> batLabel;
QList<QLabel*> balanceLabel;

QList<QString> alarmStat = {
    "","","单节过压保护","",
    "","单节低压保护","充电过流保护","放电过流保护",
    "充电高温","放电高温","充电低温","放电低温",
    "","","短路保护","",
    "","","低温单节过压保护","",
    "","","","MOS高温保护",
};
QList<QString> loseStat = {
    "电压传感器失效","温度传感器失效","充电控制异常","放电控制异常",
    "电芯异常","","","",
    "电芯寿命终止    ",
};

QList<QString> otherInfo = {
    "","加热器开启","","",
    "","","充电MOS断开","放电MOS断开",
    "","","","",
    "","","","",
    "","","","",
    "","自加热模式         ","","",
};
QList<QString> batStat = {
    "充电",
    "放电",
    "满电            ",
    "空闲",

};

QList<QString> balanceStat = {
    "电芯1","电芯2","电芯3","电芯4",
    "电芯5","电芯6","电芯7","电芯8",
    "电芯9","电芯10","电芯11","电芯12",
    "电芯13","电芯14","电芯15","电芯16          ",
};

// 输入值，输出对应字符串
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



uint32_t textDcode::CalCheckSum(QVector<uint8_t> hexVector)
{
    uint32_t checkSum = 0;
    foreach(uint8_t hexVal, hexVector) {
        checkSum += hexVal;
    }
    return checkSum;
}

// 把收到的所有数据，替换成type，ack和data.
QString textDcode::readDataDocode(QStringList hexStrLis, QString decodeStr)
{
    QStringList Command = {"地址","高字节","低字节","单板类型","功能码","握手1","握手2","ack","data","check"};
    QVector<uint8_t> hexVector;
    uint32_t dataNum = 0, dataSum = 0, datId = 0;
    QStringList codeList;
    decodeStr = "";
    bool ok;

    foreach(QString hexStr, hexStrLis) {
        hexVector.append((uint8_t)hexStr.toInt(&ok, 16));
    }
    qDebug() << "size hexStrLis = " << hexStrLis.size() << "decodeStr.lenth = " << decodeStr.length();
    for (uint32_t i = 0, limit = Command.size(); i < limit; i++) {
        qDebug() << i << "datid=" << datId << "codeList = " << hexStrLis[datId] << codeList;
        if (Command[i] == "地址") {
            // codeList.append(hexStrLis[datId]);
        } else if (Command[i] == "高字节") {
            // codeList.append(hexStrLis[datId]);
            dataNum += hexVector[datId] * 256;
            dataSum += hexVector[datId];
        } else if (Command[i] == "低字节") {
            // codeList.append(hexStrLis[datId]);
            dataNum += hexVector[datId];
            dataSum += hexVector[datId];
            if (dataNum != (uint32_t)hexStrLis.size() - 4) {
                break;
            }
        } else if (Command[i] == "功能码") {
            codeList.append(this->ByteDecode(this->funcCode, (hexVector[datId]) - 0x80));
            dataSum += hexVector[datId];
        } else if (Command[i] == "ack") {
            codeList.append(this->ByteDecode(this->ackCode, hexVector[datId]));
            dataSum += hexVector[datId];
        } else if (Command[i] == "data") {
            qDebug() << "datId" << datId << "dataNum" << dataNum << "hexstrlis size = " << hexStrLis.size();
            if (dataNum == 5) {
            } else if (dataNum > 5) {
                dataSum += this->CalCheckSum(hexVector.mid(i,dataNum - 5));
                datId += dataNum - 5;
            }
            continue;
        } else if (Command[i] == "check") {
            qDebug() << "dataSum = " << dataSum << "& 0xff = " << (dataSum & 0xff);
            if ((dataSum & 0xff) == hexVector[datId]) {
                codeList.append("校验正确");
            } else {
                codeList.append("校验错误");
            }
        } else {
            dataSum += hexVector[datId];
        }
        datId++;
    }

    if(hexStrLis.size() > 8 && (hexStrLis[4] == "93")) {
        // 把数据写入tbsUnit
        HexWriteDataStruct(hexStrLis.mid(8, 96), tbsStru0); // 需要改成自适应
    } else if (hexStrLis.size() > 8 && (hexStrLis[4] == "96")) {
        HexWriteTver(hexStrLis.mid(8, 80), tverStru0);
    } else if (hexStrLis.size() > 8 && (hexStrLis[4] == "87")) {
        HexWriteDataStruct(hexStrLis.mid(8, caliStru0->dataLenth), caliStru0);
    } else {
        qDebug() << "dataList <= 8 or datalist[4] != 93";
    }

    qDebug() << "codelist.join = " << codeList.join(COMUT_SEP);
    return codeList.join(COMUT_SEP);
}

// 将除了data的所有hex字节数据用 字符 替换。
QString textDcode::SendCmdDocode(QStringList hexStrLis, QString decodeStr)
{
    QString Command[8] = {"地址","低字节","高字节","单板类型","功能码","握手1","握手2","check"};
    QString strm;
    QStringList outStrL;

    bool ok;
    for (uint32_t i = 0, limit = hexStrLis.size(); i < limit; i++) {
        qDebug() << hexStrLis[i].toInt(&ok, 16) << decodeStr;
        if (Command[i] == "功能码") {
            outStrL.append(this->ByteDecode(this->funcCode, hexStrLis[i].toInt(&ok, 16)));
        } else if (Command[i] == "ack") {
            outStrL.append(this->ByteDecode(this->ackCode, hexStrLis[i].toInt(&ok, 16)));
        }
    }
    return outStrL.join(COMUT_SEP);
}

// 给输入str，加上时间戳
QString textDcode::AddTimeStamp(Ui::Widget *ui, QString decodeStr)
{
    QString outToPlainText;
//    Data += '\r';//插入换行
    qDebug() << ui->TimeCheckBox->isChecked() << "time checkbox";
    if (ui->TimeCheckBox->isChecked()) {
        outToPlainText = QString("[%1]:TX&->").arg(QTime::currentTime().toString("HH:mm:ss:zzz")) + COMUT_BAT_SEP + decodeStr;
    }
    ui->receiveData->appendPlainText(outToPlainText);

    return outToPlainText;
}

// 将plaintext 最后一个block内的可解析数据替换
QString textDcode::PlainTextDecode(Ui::Widget *ui)
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
    qDebug() << "doc" << doc;
    dataText = textBlock.text();
    timeText = "";
    qDebug() << "dataText" << dataText;
    // if (!dataText.contains(COMUT_BAT_SEP, Qt::CaseSensitive)) {
    //     // timeAndDataList = dataText.split(";"); // 和时间戳分开
        
    // }
    if (dataText.contains(COMUT_BAT_SEP, Qt::CaseSensitive)) {
        timeAndDataList = dataText.split(COMUT_BAT_SEP); // 和时间戳分开
        timeText = timeAndDataList[0];
        dataText = timeAndDataList[1];
    } else {
        return QString("数据非法,无;,") + dataText;
    }

    if (dataText.contains(QRegExp("^[0-9a-fA-F]{1,}$")) == true) {
        return QString("数据非法,包含非法字符") + COMUT_SEP + timeAndDataList[0] + COMUT_BAT_SEP + timeAndDataList[1];
    }
    qDebug()  << "text block" << textBlock.text();

    // 去除掉HEX字符串的空格，把HEX转换成UTF8字符串
    dataList = dataText.simplified().split(' ');

    qDebug() << dataList << "dataList out<<" << dataList.size()<<" list size" ;
    // 通过长度判断是send还是receive

    if (dataList.size() == 8) {
        dataText = SendCmdDocode(dataList, dataText)  + COMUT_SEP + timeAndDataList[0] + COMUT_BAT_SEP;

    } else if (dataList.size() > 8 && dataList.size() < 200) {
        dataText = readDataDocode(dataList, dataText) + COMUT_SEP + timeAndDataList[0] + COMUT_BAT_SEP;
        if(dataText.contains("产品注册")) {
            if(dataText.contains("校验正确")) {
                ui->portStatus->setText("产品注册成功");
            } else {
                ui->portStatus->setText("产品注册失败");
            }
        } else if(dataText.contains("TBS数据")) {
            if(dataText.contains("校验正确")) {
                foreach(dataCell cell, tbsStru0->dataCellList) {
                    QString tbsStr;         
                    if(cell.valName.contains("HEX")) {
                        dataText += QString("0x%1").arg(cell.uintVal,0,16) + BAT_SEP;
                    } else if(cell.valName.contains("电流")) {
                        dataText += QString("%1").arg((int)cell.uintVal,0,10) + BAT_SEP;
                    } else {
                        dataText += QString("%1").arg(cell.uintVal,0,10) + BAT_SEP;
                    }
                    // dataText += tbsStr.setNum(cell.uintVal, 10) + BAT_SEP;
                }
                ui->listWidget->addItem(dataText);
            }
        }
    } else {
        qDebug() << "数据非法长度错误";
        dataText = QString("数据非法长度错误") + dataText;
    }
    qDebug() << "split = " << timeText << dataText;

    //单独解码
    // Widget.SetTbsToTableAndChart(dataText, 1);
    return dataText;

}

// 把输入的itemlist内的值修改成空“”
void textDcode::clearTableItem(QVector<QTableWidgetItem>* itemTableList)
{
    uint32_t idx = 0;
    foreach(dataCell cell, tbsStru0->dataCellList) {
        (*itemTableList)[idx * 2 + 1].setText("");
        idx++;
    }
}

// 把输入的itemlist内写入新的值
void textDcode::itemToTable(QVector<QTableWidgetItem>* itemTableList)
{
    uint32_t idx = 0;
    foreach(dataCell cell, tbsStru0->dataCellList) {
        (*itemTableList)[idx * 2].setText(cell.valName);
         if(cell.valName.contains("HEX")) {
             (*itemTableList)[idx * 2 + 1].setText(QString("0x%1").arg(cell.uintVal,0,16));
         } else if(cell.valName.contains("电流")) {
             (*itemTableList)[idx * 2 + 1].setText(QString("%1").arg((int)cell.uintVal,0,10));
         } else {
             (*itemTableList)[idx * 2 + 1].setText(QString("%1").arg(cell.uintVal,0,10));
         }

        idx++;
    }
}

// // 将字符串str转换成真实的int值，再转换成str写入tbsUnit
// QVector<tbs> textDcode::HexWriteTbs(QStringList dataList)
// {

//     bool ok;
//     QVector<uint8_t> hexVector;
//     uint8_t byteInData = 0;
//     uint32_t tbsUnitIdx = 0, uintVal = 0;

//     foreach(QString hexStr, dataList) {
//         hexStr.toInt(&ok, 16);
//         if (ok == true) {
//             hexVector.append((uint8_t)hexStr.toInt(&ok, 16));
//         }
//     }
//     foreach(uint8_t hex, hexVector) {
//         if (byteInData == tbsUnit[tbsUnitIdx].typeLenth) {
//             tbsUnit[tbsUnitIdx].uintVal = uintVal;
//             // qDebug() << tbsUnit[tbsUnitIdx].valName <<"uintval%d = " << uintVal;

//             tbsUnitIdx++;
//             uintVal = 0;
//             byteInData = 0;
//         }
//         uintVal += (((uint32_t)hex) <<  byteInData * 8);
//         byteInData++;
//     }
//     return tbsUnit;
// }
void textDcode::IntWriteTbs(QStringList dataList)
{

    bool ok;
    QVector<uint8_t> hexVector;
    uint32_t tbsUnitIdx = 0, uintVal = 0;

    foreach(QString hexStr, dataList) {
        if(tbsStru0->dataCellList[tbsUnitIdx].valName.contains("HEX")) {
            uintVal = hexStr.toUInt(&ok, 16);
            tbsStru0->dataCellList[tbsUnitIdx].uintVal = uintVal;
        } else if(tbsStru0->dataCellList[tbsUnitIdx].valName.contains("电流")) {
            uintVal = hexStr.toUInt(&ok, 10);
            tbsStru0->dataCellList[tbsUnitIdx].uintVal = uintVal;
        } else {
            uintVal = hexStr.toUInt(&ok, 10);
            tbsStru0->dataCellList[tbsUnitIdx].uintVal = uintVal;
        }

        tbsUnitIdx++;
        if(tbsUnitIdx >= (uint32_t)tbsStru0->dataCellList.size()) {
            qDebug() << "tbsUnit.size()" << tbsStru0->dataCellList.size();
            break;
        }
    }
    return;
}

// 将字符串str转换成真实的int值，再转换成str写入tverStruct
QString textDcode::HexWriteTver(QStringList dataList, tverStruct* tverStuObject)
{

    bool ok;
    QVector<uint8_t> hexVector;
    uint8_t byteInUnit = 0;
    uint32_t  uintVal = 0;

    foreach(QString hexStr, dataList) {
        hexStr.toInt(&ok, 16);
        if (ok == true) {
            hexVector.append((uint8_t)hexStr.toInt(&ok, 16));
        }
    }

    if((uint32_t)hexVector.size() != tverStuObject->dataLenth) {
        return "tver数据长度不对";
    }

    QList<QString>::iterator it;
    it = tverStuObject->keyList.begin();
    tver tver0 = tverStuObject->tverMap.value(*it);
    foreach(uint8_t hex, hexVector) {
        uintVal += (((uint32_t)hex) <<  (byteInUnit * 8));
        tver0.byteArray.append(hex);
        byteInUnit++;
        if(byteInUnit == tver0.typeLenth) {
            tver0.uintVal = uintVal;
            uintVal = 0;
            byteInUnit = 0;
            qDebug() << tver0.valName << tver0.uintVal << tver0.byteArray;
            tverStuObject->tverMap.insert(*it,tver0);
            qDebug() << tverStuObject->tverMap.value(*it).valName << tverStuObject->tverMap.value(*it).uintVal << tverStuObject->tverMap.value(*it).byteArray;
            it++;
            if(it == tverStuObject->keyList.end()){
                break;
            }
            tver0 = tverStuObject->tverMap.value(*it);
        }
    }
    tverStuObject->newDataStatus = true;
    return "tver解析正确";
}
//// 将字符串str转换成真实的int值，再转换成str写入dataStruct
QString textDcode::HexWriteDataStruct(QStringList dataList, dataStruct* struObject)
{

    bool ok;
    QVector<uint8_t> hexVector;
    uint8_t byteInUnit = 0;
    uint32_t  uintVal = 0;

    foreach(QString hexStr, dataList) {
        hexStr.toInt(&ok, 16);
        if (ok == true) {
            hexVector.append((uint8_t)hexStr.toInt(&ok, 16));
        }
    }

    if((uint32_t)hexVector.size() != struObject->dataLenth) {
        return "tver数据长度不对";
    }

    QList<QString>::iterator it;
    it = struObject->keyList.begin();
    dataCell* cell0 = struObject->value(*it);
    cell0->bigEndianBArray.clear();
    cell0->byteArray.clear();
    foreach(uint8_t hex, hexVector) {
        uintVal += (((uint32_t)hex) <<  (byteInUnit * 8));
        cell0->byteArray.append(hex);
        cell0->bigEndianBArray.insert(0, hex);
        byteInUnit++;
        if(byteInUnit == cell0->typeLenth) {
            cell0->uintVal = uintVal;
            uintVal = 0;
            byteInUnit = 0;
            qDebug() << struObject->value(*it)->valName << struObject->value(*it)->uintVal << struObject->value(*it)->byteArray;
            it++;
            if(it == struObject->keyList.end()){
                break;
            }
            cell0 = struObject->value(*it);
            cell0->bigEndianBArray.clear();
            cell0->byteArray.clear();
        }
        qDebug() << cell0->valName << cell0->uintVal << cell0->byteArray;
    }
    struObject->newDataStatus = true;
    return "tver解析正确";
}
// 从输入的item判断数据是否是tbs数据，如果是则解析再写入到tbsUnit
bool textDcode::ItemToTbs(QString text)
{
    QStringList timeAndDataList, dataList;

    timeAndDataList = text.split(COMUT_BAT_SEP);
    if(timeAndDataList.begin()->contains("TX") || timeAndDataList.size() < 2) {
        return false;
    }

    dataList = timeAndDataList[1].split(BAT_SEP); // 需要改，防止溢出
    qDebug() << "datalist = " << dataList;
    // if(dataList.size() <= 8 || (dataList[4] != "93" && dataList[4] != "bat状态")) {
    //     qDebug() << "dataList <= 8 or datalist[4] != 93";
    //     return false;
    // }
    if(dataList.size() != 37) {
        qDebug() << dataList.size();
    }
    // 把数据写入tbsUnit
    this->IntWriteTbs(dataList); // 需要改成自适应
    return true;
}

// 把告警状态，保护状态，电池状态，失效状态，其他状态写入到groupBox中
void textDcode::SetStatusToBox(Ui::Widget *ui)
{
    this->SetStatusToGBox(ui->gridLayout_7);
    this->SetStatusToLBox(ui->loseGridLayout,  loseStat,  loseLabel,  tbsStru0->dataCellList[30].uintVal);
    this->SetStatusToLBox(ui->otherGridLayout, otherInfo, otherLabel, tbsStru0->dataCellList[27].uintVal);
    this->SetStatusToLBox(ui->batGridLayout,   batStat,   batLabel,    tbsStru0->dataCellList[32].uintVal);
    this->SetStatusToLBox(ui->balanceGridLayout,   balanceStat,   balanceLabel,    tbsStru0->dataCellList[31].uintVal);

//    this->SetStatusToLBox(ui->batGridLayout,   batStat,   batlabel,    tbsUnit[33].uintVal);
}

// 把告警状态和保护状态写入到groupBox中
void textDcode::SetStatusToGBox(QGridLayout *gridLayout)
{
    int i = 0;
    // qDebug() << " protect status" << tbsUnit[29].uintVal;
    int bitNum = -1;
    foreach(QString statName, alarmStat)
    {
        bitNum++;
        if(statName == "") {
            continue;
        }
        int iX3 = i * 3;
        if(alarmLabel.value(iX3) == 0) {
            alarmLabel.append(new QLabel());
            alarmLabel.append(new QLabel());
            alarmLabel.append(new QLabel());

            gridLayout->addWidget(alarmLabel[iX3],   i, 0);
            gridLayout->addWidget(alarmLabel[iX3+1], i, 1);
            gridLayout->addWidget(alarmLabel[iX3+2], i, 2);
            alarmLabel[iX3 + 1]->resize(50,50);
            alarmLabel[iX3 + 2]->resize(50,50);
        }

        alarmLabel[iX3]->setText(statName);
        if((tbsStru0->dataCellList[28].uintVal & 0x1 << bitNum) != 0) {
            alarmLabel[iX3 + 1]->setStyleSheet("QLabel { background-color: red}");
        } else {
            alarmLabel[iX3 + 1]->setStyleSheet("QLabel { background-color: green}");
        }
        if((tbsStru0->dataCellList[29].uintVal & 0x1 << bitNum) != 0) {
            alarmLabel[iX3 + 2]->setStyleSheet("QLabel { background-color: red}");
        } else {
            alarmLabel[iX3 + 2]->setStyleSheet("QLabel { background-color: green}");
        }
        i++;
    }
}

// 把电池状态，失效状态，其他状态写入到groupBox中
void textDcode::SetStatusToLBox(QGridLayout *gridLayout, QList<QString> strL, QList<QLabel*> labelL, uint32_t val)
{
    int i = 0;
    int bitNum = -1;
    // qDebug() << "status val" << val;

    foreach(QString statName, strL)
    {
        bitNum++;
        if(statName == "") {
            continue;
        }

        int iX3 = i * 2;
        if(labelL.value(iX3) == 0) {
            labelL.append(new QLabel());
            labelL.append(new QLabel());
            // 偶数是名称，奇数是状态
            gridLayout->addWidget(labelL[iX3],     i, 0);
            gridLayout->addWidget(labelL[iX3 + 1], i, 1);
            labelL[iX3 + 1]->resize(50,50);
        }

        labelL[iX3]->setText(statName);
        // 空闲状态值为0x00，与其他状态相反
        if(statName == "空闲") {
            if(val == 0x0) {
                labelL[iX3 + 1]->setStyleSheet("QLabel { background-color: red}");
            } else {
                labelL[iX3 + 1]->setStyleSheet("QLabel { background-color: green}");
            }
            break;
        }
        if((val & (0x1 << bitNum)) != 0) {
            labelL[iX3 + 1]->setStyleSheet("QLabel { background-color: red}");
        } else {
            labelL[iX3 + 1]->setStyleSheet("QLabel { background-color: green}");
        }

//        qDebug() << i << iX3;
        i++;
    }
}

QString textDcode::GetTime(QString csvData)
{
    int left = csvData.indexOf("[") + 1;
    int right = csvData.indexOf("]") - 1;
    return csvData.mid(left, right - left + 1);
}

