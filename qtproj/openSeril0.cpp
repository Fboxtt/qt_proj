#include <serial0.h>

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

    // 波特率    //波特率默认选择下拉第三项：9600
    baudList<<"1200"<<"2400"<<"4800"<<"9600"
           <<"14400"<<"19200"<<"38400"<<"56000"
          <<"57600"<<"115200";
    ui->rateBox->addItems(baudList);
    ui->rateBox->setCurrentIndex(3);

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
    }
}
