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

    this->tbsUnion = &tbsUnit;
}

QList<QLabel*> alarmLabel;
QList<QLabel*> loseLabel;
QList<QLabel*> otherLabel;
QList<QLabel*> batlabel;

QList<QString> alarmStat = {
    "Pack过压",
    "电池组过压",
    "单体电芯过压",
    "电池组欠压",
    "单体电芯欠压",
    "充电过流",
    "放电过流",
    "充电高温",
    "放电高温",
    "充电低温",
    "放电低温",
    "容量低",
    "放电短路",
    "反接",
    "低温充电过流",
    "低温充电过流",
    "低温充电过压",
    "充电短路",
};
QList<QString> loseStat = {
    "电压传感器失效",
    "温度传感器失效",
    "充电管失效",
    "放电管失效",
    "电芯损坏",
    "加热器失效",
    "面板开关关闭",
    "限流充电回路失效",
    "保险丝熔断失效",
};
QList<QString> otherStat = {
    "加热器",
    "满充计时",
    "电压校准",
    "电流校准",
    "充电限流",

};
QList<QString> batStat = {
    "空闲",
    "充电",
    "放电",
    "满电",

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
            // codeList.append(hexStrLis[datId]);
            dataSum += hexVector[datId];
        }
        datId++;
    }
    qDebug() << "codelist.join = " << codeList.join(",");
    return codeList.join(",");
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
    return outStrL.join(",");
}

// 给输入str，加上时间戳
QString textDcode::AddTimeStamp(Ui::Widget *ui, QString decodeStr)
{
    QString outToPlainText;
//    Data += '\r';//插入换行
    qDebug() << ui->TimeCheckBox->isChecked() << "time checkbox";
    if (ui->TimeCheckBox->isChecked()) {
        outToPlainText = QString("[%1]:TX&->").arg(QTime::currentTime().toString("HH:mm:ss:zzz")) + decodeStr;
    }
    ui->receiveData->appendPlainText(outToPlainText);

    return outToPlainText;
}

// 将plaintext 最后一个block内的可解析数据替换
QString textDcode::TextDecode(Ui::Widget *ui)
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
        return QString("数据非法") +"," + timeAndDataList[0] + "->" + timeAndDataList[1];
    }
    qDebug() << textBlock.text() << "text block";

    dataList = dataText.simplified().split(' ');

    qDebug() << dataList << "dataList out<<" << dataList.size()<<" list size" ;
    // 通过长度判断是send还是receive

    if (dataList.size() == 8) {
        dataText = SendCmdDocode(dataList, dataText)  + "," + timeAndDataList[0] + "->";

    } else if (dataList.size() > 8 && dataList.size() < 200) {
        dataText = readDataDocode(dataList, dataText) + "," + timeAndDataList[0] + "->";

    } else {
        qDebug() << "无法解析";
        dataText = QString("无法解析") + "->" ;
        // error
    }
    qDebug() << "split = " << timeText << dataText;
    //单独解码

    //返回并显示
    ui->listWidget->addItem(dataText);
    return "";

}

// 把输入的itemlist内的值修改成空“”
void textDcode::clearTableItem(QVector<QTableWidgetItem>* itemTableList)
{
    uint32_t idx = 0;
    foreach(tbs unit, tbsUnit) {
        (*itemTableList)[idx * 2 + 1].setText("");
        idx++;
    }
}

// 把输入的itemlist内写入新的值
void textDcode::itemToTable(QVector<QTableWidgetItem>* itemTableList)
{
    uint32_t idx = 0;
    foreach(tbs unit, tbsUnit) {
        (*itemTableList)[idx * 2].setText(unit.valName);
        (*itemTableList)[idx * 2 + 1].setText(QString("%1").arg(unit.uintVal,0,10));
        idx++;
    }
}

// 将字符串str转换成真实的int值，再转换成str写入tbsUnit
QVector<tbs> textDcode::HexToStr(QStringList dataList)
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
    foreach(uint8_t hex, hexVector) {
        if (byteInData == tbsUnit[tbsUnitIdx].typeLenth) {
            tbsUnit[tbsUnitIdx].uintVal = uintVal;
            // qDebug() << tbsUnit[tbsUnitIdx].valName <<"uintval%d = " << uintVal;

            tbsUnitIdx++;
            uintVal = 0;
            byteInData = 0;
        }
        uintVal += (((uint32_t)hex) <<  byteInData * 8);
        byteInData++;
    }
    return tbsUnit;
}


// 从输入的item判断数据是否是tbs数据，如果是则解析再写入到tbsUnit
bool textDcode::ItemToTbs(QListWidgetItem *item)
{
    QStringList timeAndDataList, dataList;

    timeAndDataList = item->text().split("->");
    if(timeAndDataList.begin()->contains("TX") || timeAndDataList.size() < 2) {
        return false;
    }

    dataList = timeAndDataList[1].split(" "); // 需要改，防止溢出
    if(dataList.size() <= 8 || (dataList[4] != "93" && dataList[4] != "bat状态")) {
        qDebug() << "dataList <= 8 or datalist[4] != 93";
        return false;
    }

    // 把数据写入tbsUnit
    QVector<tbs> decodeList = this->HexToStr(dataList.mid(8, -1)); // 需要改成自适应
    return true;
}

// 把告警状态，保护状态，电池状态，失效状态，其他状态写入到groupBox中
void textDcode::SetStatusToBox(Ui::Widget *ui)
{
    this->SetStatusToGBox(ui->gridLayout_7);
    this->SetStatusToLBox(ui->loseGridLayout,  loseStat,  loseLabel,  tbsUnit[31].uintVal);
    this->SetStatusToLBox(ui->otherGridLayout, otherStat, otherLabel, tbsUnit[32].uintVal);
    this->SetStatusToLBox(ui->batGridLayout,   batStat,   batlabel,    tbsUnit[33].uintVal);
}

// 把告警状态和保护状态写入到groupBox中
void textDcode::SetStatusToGBox(QGridLayout *gridLayout)
{
    int i = 0;
    qDebug() << " protect status" << tbsUnit[29].uintVal;

    foreach(QString statName, alarmStat)
    {
        int iX3 = i * 3;
        if(alarmLabel.value(iX3) == 0) {
            alarmLabel.append(new QLabel());
            alarmLabel.append(new QLabel());
            alarmLabel.append(new QLabel());

            gridLayout->addWidget(alarmLabel[iX3],   i, 0);
            gridLayout->addWidget(alarmLabel[iX3+1], i, 1);
            gridLayout->addWidget(alarmLabel[iX3+2], i, 2);
        }

        alarmLabel[iX3]->setText(statName);
        if((tbsUnit[28].uintVal & 0x1 << i) != 0) {
            alarmLabel[iX3 + 1]->setStyleSheet("QLabel { background-color: red}");
        } else {
            alarmLabel[iX3 + 1]->setStyleSheet("QLabel { background-color: green}");
        }
        if((tbsUnit[29].uintVal & 0x1 << i) != 0) {
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
    qDebug() << "status val" << val;

    foreach(QString statName, strL)
    {
        int iX3 = i * 2;
        if(labelL.value(iX3) == 0) {
            labelL.append(new QLabel());
            labelL.append(new QLabel());

            gridLayout->addWidget(labelL[iX3],     i, 0);
            gridLayout->addWidget(labelL[iX3 + 1], i, 1);
        }

        labelL[iX3]->setText(statName);

        if((val & 0x1 << i) != 0) {
            labelL[iX3 + 1]->setStyleSheet("QLabel { background-color: red}");
        } else {
            labelL[iX3 + 1]->setStyleSheet("QLabel { background-color: green}");
        }
        qDebug() << i << iX3;
        i++;
    }
}

// 把从csv中读取到的行数据写入tbs中
bool textDcode::CsvToTbs(QByteArray csvData)
{
    QString tbsStr = QString::fromLocal8Bit(csvData);
    tbsStr.remove(QChar::LineFeed);
    tbsStr.remove(QChar::CarriageReturn);
    QStringList strList = tbsStr.split(",");
    if(strList.value(-1) == "")
    {
        strList.removeLast();
    }
    if(tbsStr.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        return false;
    }
    if(strList.size() != tbsUnit.size())
    {
        qDebug() << "csvstring size error" << strList.size() << tbsUnit.size() << strList;

        return false;
    }
    int i = 0;
    bool ok = false;
    foreach(QString str, strList){
        tbsUnit[i].uintVal = str.toInt(&ok, 10);
        qDebug() << tbsUnit[i].uintVal;
        i++;
    }

    return true;
}
