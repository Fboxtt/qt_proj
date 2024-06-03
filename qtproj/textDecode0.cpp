#include <textDecode0.h>


textDcode::textDcode(void)
{
    funcCode =  {{0x00,"产品注册"} \
                ,{0x01,"断开注册"}\
                ,{0x13,"bat状态"}\
                ,{0x0A,"开chg fet"}\
                ,{0x0B,"关chg fet"}\
                ,{0x0C,"开dchg fet"}\
                ,{0x0D,"关dchg fet"}\
                ,{0x10,"获取产品序列号"}\
                ,{0x16,"获取版本"}\
                ,{0x60,"关机命令"}\
                ,{0x64,"休眠命令"}};

    ackCode =   {{0x00,"无异常"}\
                ,{0x01,"长度错误"}\
                ,{0x03,"类型错误"}\
                ,{0x04,"id错误"}\
                ,{0x05,"握手错误"}\
                ,{0x06,"校验错误"}};;
}

QString textDcode::ByteDecode(QMap<int, QString> mapCode, int keys)
{
    QString strCode;
    if (!mapCode.contains(keys)) {
        strCode =  "--";
        return strCode;
    }
    strCode = mapCode.value(keys);
    return strCode;
}

QString textDcode::SendCmdDocode(QStringList hexList, QString decodeStr)
{
    QString Command[9] = {"地址","低字节","高字节","单板类型","功能码","握手1","握手2","ack","check"};
    QString strm;
    decodeStr = "";
    bool ok;
    for (int i = 0, limit = hexList.size(); i < limit; i++) {
        qDebug() << hexList[i].toInt(&ok, 16) << decodeStr;

        if (Command[i] == "功能码") {
            decodeStr.append(this->ByteDecode(this->funcCode, hexList[i].toInt(&ok, 16)) + " ");
        } else if (Command[i] == "ack") {
            decodeStr.append(this->ByteDecode(this->ackCode, hexList[i].toInt(&ok, 16)) + " ");
        } else {
            decodeStr.append(hexList[i] + " ");
        }
    }
    qDebug() << "endstr:" << decodeStr;
    return decodeStr;
}

QString textDcode::DecodeHexToCommand(Ui::Widget *ui, QString decodeStr)
{
    QString Command[10] = {"地址","低字节","高字节","单板类型","功能码","握手1","握手2","ack","data","check"};
    QString log;
    QTextBlock textBlock;
    QTextDocument *doc;
    QByteArray textByte;
    QStringList textList;
    int blockCount;

//    Command[0] = "address";
//    ui->receiveData->setPlainText("00 00 05 01 81 55 AA 00");
    ui->receiveData->setPlainText("00 00 04 01 01 55 AA 06");
//    ui->HexEncodeText->appendPlainText(ui->receiveData->);
    qDebug() << ui->receiveData->blockCount() << "block count";

    doc = ui->receiveData->document();

    blockCount = ui->receiveData->blockCount();

    textBlock = doc->findBlockByNumber(blockCount - 1);


    log = textBlock.text();

    if (log.contains(QRegExp("^[0-9a-fA-F]{1,}$")) == true) {
        decodeStr = "数据非法";
        return decodeStr;
    }
    qDebug() << textBlock.text() << "text block";
    textList = log.split(' ');

    qDebug() << textList << "textlist out<<" <<" list size" << textList.size();
    // 通过长度判断是send还是receive

    if (textList.size() == 8) {
        decodeStr = SendCmdDocode(textList,decodeStr);
        // send data
    } else if (textList.size() > 8) {
        // receive data
    } else {
        qDebug() << "无法解析";
        // error
    }
    qDebug() << "2 endstr" << decodeStr;
    //单独解码

    //返回并显示
    return decodeStr;

}

