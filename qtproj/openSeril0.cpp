#include <serial0.h>
#include <QTimer>
#include <QTime>
#include <QDateTime>

serial::serial(void)
{

}

void serial::Init(Ui::Widget *ui)
{
    //读取串口信息
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //访问静态成员函数，返回Qlist
    {
        // 自动读取串口号添加到端口portBox中
        QSerialPort serial;
        serial.setPort(info);
        qDebug() << serial.portName() << "serial init debug";
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->portBox->addItem(info.portName());
            serial.close();
        }
    }
    QSerialPortInfo::availablePorts()[0];
    QStringList baudList;   //波特率
    QStringList parityList; //校验位
    QStringList dataBitsList;   //数据位
    QStringList stopBitsList;   //停止位

    // 波特率    //波特率默认选择下拉第五项：19200
    baudList<<"1200"<<"2400"<<"4800"<<"9600"
           <<"14400"<<"19200"<<"38400"<<"56000"
          <<"57600"<<"115200";
    ui->rateBox->addItems(baudList);
    ui->rateBox->setCurrentIndex(5);

    // 校验      //校验默认选择无
    parityList<<"无"<<"奇"<<"偶";
    ui->checkBox->addItems(parityList);
    ui->checkBox->setCurrentIndex(0);

    // 数据位      //数据位默认选择8位
    dataBitsList<<"5"<<"6"<<"7"<<"8";
    ui->dataBox->addItems(dataBitsList);
    ui->dataBox->setCurrentIndex(3);

    // 停止位      //停止位默认选择1位
    stopBitsList<<"1"<<"2";
    ui->stopBox->addItems(stopBitsList);
    ui->stopBox->setCurrentIndex(0);
}

void serial::RefreshSerial(Ui::Widget *ui)
{
    ui->portBox->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //访问静态成员函数，返回Qlist
    {
        // 自动读取串口号添加到端口portBox中
        QSerialPort serial;
        serial.setPort(info);
        qDebug() << serial.portName() << "serial init debug";
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->portBox->addItem(info.portName());
            serial.close();
        }
    }
}

void serial::ClickOpenSerPort(Ui::Widget *ui)
{
    // 打开串口
    if(ui->openBtn->text() == "打开串口")
    {
        // 设置串口号
        SerialPort.setPortName(ui->portBox->currentText());
        // 打开串口
        if(SerialPort.open(QIODevice::ReadWrite))
        {
            // 设置波特率
            SerialPort.setBaudRate(ui->rateBox->currentText().toInt());
            //设置数据位数
            switch(ui->dataBox->currentIndex())
            {
            case 5: SerialPort.setDataBits(QSerialPort::Data5); break;
            case 6: SerialPort.setDataBits(QSerialPort::Data6); break;
            case 7: SerialPort.setDataBits(QSerialPort::Data7); break;
            case 8: SerialPort.setDataBits(QSerialPort::Data8); break;
            default: break;
            }
            // 设置校验位
            //SerialPort->setParity(QSerialPort::NoParity);
            //设置奇偶校验
            switch(ui->checkBox->currentIndex())
            {
            case 0: SerialPort.setParity(QSerialPort::NoParity); break;
            case 1: SerialPort.setParity(QSerialPort::OddParity); break;
            case 2: SerialPort.setParity(QSerialPort::EvenParity); break;
            default: break;
            }
            // 设置流控制
            SerialPort.setFlowControl(QSerialPort::NoFlowControl);
            //设置停止位
            switch(ui->stopBox->currentIndex())
            {
            case 1: SerialPort.setStopBits(QSerialPort::OneStop); break;
            case 2: SerialPort.setStopBits(QSerialPort::TwoStop); break;
            default: break;
            }
            ui->portStatus->setText("串口已连接");
        }
        //打开串口
        else
        {
//            QMessageBox::about(NULL, "提示", "串口无法打开\r\n不存在或已被占用");
            ui->portStatus->setText("无法打开或不存在");
            return;
        }
        ui->openBtn->setText("关闭串口");
        //下拉菜单控件使能
        ui->portBox->setEnabled(false);
        ui->rateBox->setEnabled(false);
        ui->dataBox->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->stopBox->setEnabled(false);
        //搜索串口按键使能
        ui->searchBtn->setEnabled(false);
        //发送按键使能
        ui->sendBox->setEnabled(true);
        // 读取tbs按键使能
        ui->pushButton_3->setEnabled(true);
    }
    // 关闭串口
    else
    {
        SerialPort.close();
        ui->openBtn->setText("打开串口");
        //下拉按键使能
        ui->portBox->setEnabled(true);
        ui->rateBox->setEnabled(true);
        ui->dataBox->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->stopBox->setEnabled(true);
        //搜索串口按键使能
        ui->searchBtn->setEnabled(true);
        //发送按键使能
        ui->sendBox->setEnabled(false);
        // 读取tbs按键关闭
        ui->pushButton_3->setEnabled(false);
        ui->portStatus->setText("串口未连接");
    }
}
void serial::TimeOut(Ui::Widget *ui, QTimer *readTim)
{
    QString utf8Buffer;
    //从缓冲区中读取数据
    QByteArray buffer = SerialPort.readAll();
    QByteArray testbu ;

    qDebug() << buffer << "serial read buffer";
    QString gbkBuffer;
    if(!buffer.isEmpty())//如果非空说明有数据接收
    {   //转换成16进制大写
        QString str=buffer.toHex().data();
        str=str.toUpper();
        
        gbkBuffer = QString::fromLocal8Bit(buffer);
        //一个16进制占4位，8位为一字节，所以每两位16进制空一格
        qDebug() << "7.0.1把字符串中间添加空格";
        for(int i=0;i<str.length();i+=2)
        {
               QString str_1 = str.mid (i,2);
               utf8Buffer += str_1;
               utf8Buffer += " ";
        }
        // utf8Buffer = utf8Buffer.simplified();
        //读取之前显示数据
        QString receive = ui->receiveData->toPlainText();
        //清空显示
        ui->receiveData->clear();
        //重新显示

//        if(serial::batComSendStatus == serial::COMPLETE) // 加换行符
//        {
            qDebug() << "complete func in" << "\r";
            receive += "\r";
//        }
        if (ui->TimeCheckBox->isChecked()) // 加时间戳
        {
            receive += QString("[%1]:RX ->").arg(QTime::currentTime().toString("HH:mm:ss:zzz")) + COMUT_BAT_SEP;
        }
        if(ui->hexDisplay->checkState() == Qt::Unchecked){ // 加数据
            receive += QString(gbkBuffer);
        }//直接显示
        else{
            receive += QString(utf8Buffer);
        }//16进制显示
        qDebug() << "7.0.2信息框添加处理后的数据";
        ui->receiveData->appendPlainText(receive);
//        serial::batComSendStatus = serial::INCOMPLETE;
    }
    readTim->stop(); // 开启定时器，如果100ms后没有收到数据，则换行；
    buffer.clear();
}

void serial::ReadyRead(QTimer *readTim)
{
//    serial::batComSendStatus = serial::COMPLETE;
    readTim->start();
}

QString serial::SerialSend(Ui::Widget *ui, QString Data)
{
    QByteArray Data_1;
    //获取输入窗口sendData的数据
    if(ui->hexSendBox->checkState() == 0) {
        Data_1 = Data.toUtf8();//转换成utf8格式的字节流发送
    } else {
        Data_1 = QByteArray::fromHex(Data.toLatin1().data());//按十六进制编码发送
        Data = Data.remove(QChar('\n'), Qt::CaseInsensitive);
    }
    return this->SerialSend(ui, Data_1);
}


QString serial::SerialSend(Ui::Widget *ui, QByteArray sendArray)
{

    uint8_t checkSum = 0;
    if(sendArray.size() >= 8) {
        for(int i = 1, limit = sendArray.size() - 2; i <= limit; i++) {
            checkSum += sendArray[i];
        }
        if((uint8_t)sendArray[sendArray.size() - 1] != checkSum) {
            ui->portStatus->setText("发送校验错误，正确是0x" + QString::number(checkSum, 16));
        }
    }
    qDebug() << "4===========数据发送开始" << QTime::currentTime();
    SerialPort.write(sendArray);
    qDebug() << "5===========数据发送结束" << QTime::currentTime();
    QString noSpaceData = sendArray.toHex().toUpper().data(), SpaceData;
    for(int i = 0, limit = sendArray.size(); i < limit; i++) {
        if(i < limit - 1) {
            SpaceData += noSpaceData.mid(i * 2, 2) + ' ';
        } else {
            SpaceData += noSpaceData.mid(i * 2, 2);
        }
    }

    return SpaceData;
}
