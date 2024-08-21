#include "widget.h"
#include "ui_widget.h"
#include "textDecode0.h"

#include <QDebug>
#include <QFileDialog>
#include <QStringListModel>
#include <QMenu>
#include <QClipboard>
#include <QDateTime>

#include <QPainter>
#include <QPixmap>
//QT_CHARTS_USE_NAMESPACE
//using namespace QTCharts;

#include "serial0.h"
#include "csv1.h"
#include "chart.h"
#include "hexDecode.h"

serial se;
textDcode dcode0;
QVector<QTableWidgetItem> itemTableList(80);

chartV* chartV0;
hexDecode hexFile;

tverStruct *tverStru0;
caliStruct *caliStru0;
tbsStruct *tbsStru0;
tbsStruct *testLCD;

QStringList waitSendList;
QStringList readySendList;
QStringList hexSendList;
QVector<QByteArray> byteSendList;
QTimer *sendTim;
QTimer *readTim;

sysStruct *sysStru0;

#define SEND_INTERVAL 20
#define SEND_TBS_COUNT (1000/SEND_INTERVAL)
#define DELAY_READ_TIME 50
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    se.Init(ui);
    ui->sendBox->setEnabled(false);

    // 设置listWidget右键菜单
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu); 
    // 链接右击和on_PopupRightMenu槽函数
    connect(ui->listWidget,&QListWidget::customContextMenuRequested,this,&Widget::on_PopupRightMenu);



    // 设置listwidget最大宽度
    // ui->listWidget->setMaximumWidth(300);
    // listWidget设置
    ui->listWidget->setWordWrap(true); // 设置可以换行 listwidget格式设置
    ui->listWidget->setStyleSheet("QListWidget{font-size:10px;}"); // 设置字体大小 listwidget格式设置

    // tableWidget设置
    ui->tableWidget->setFont(QFont("黑体", 7)); // table字体设置
    ui->tableWidget->setFixedSize(600,800);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setVerticalHeaderLabels({"","","","","","","","","","", \
                                             "","","","","","","","","",""});
    for(int idx = 0, limit = itemTableList.size(); idx < limit; idx++) {
//        itemTableList[idx].setText(QString("%1").arg(idx,0,10));
        ui->tableWidget->setItem(idx / 4, idx % 4, &itemTableList[idx]);
    }
    // k值校准tableWidget设置
    ui->tableWidget_2->setFont(QFont("黑体", 7)); // table字体设置
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自动适应列宽

    // 波形chart配置
    chartV0 = new chartV();
    chartV0->addNewChart(ui->tab_2,"总电压",   "pack电压", "mV");
    chartV0->addNewChart(ui->tab_2,"总电压",   "电芯总压", "mV");
    chartV0->addNewChart(ui->tab_2,"电芯电压", "电芯1",    "mV");
    chartV0->addNewChart(ui->tab_2,"电芯电压", "电芯2",    "mV");
    chartV0->addNewChart(ui->tab_2,"电芯电压", "电芯3",    "mV");
    chartV0->addNewChart(ui->tab_2,"电芯电压", "电芯4",    "mV");
    chartV0->addNewChart(ui->tab_2,"温度",     "ntc1",     "C");
    chartV0->addNewChart(ui->tab_2,"容量",     "RM",       "AH");

    ui->gridLayout_5->addWidget(chartV0->chartMap.value("总电压")->chartview,   0, 0);
    ui->gridLayout_5->addWidget(chartV0->chartMap.value("电芯电压")->chartview, 0, 1);
    ui->gridLayout_5->addWidget(chartV0->chartMap.value("温度")->chartview,     1, 0);
    ui->gridLayout_5->addWidget(chartV0->chartMap.value("容量")->chartview,     1, 1);

    // 状态盒默认尺寸设置
    ui->alarmGroupBox->setMinimumWidth(300);

    ui->alarmGroupBox->setMaximumWidth(400);
    ui->groupBox_3->setMaximumWidth(400);

    // 设置状态颜色提示信息，红色和绿色代表什么颜色
    ui->label_8->setStyleSheet("QLabel { background-color: green}");
    ui->label_10->setStyleSheet("QLabel { background-color: red}");


    // 数据结构体初始化函数
    tverStru0 = new tverStruct();
    caliStru0 = new caliStruct();
    tbsStru0 = new tbsStruct();
    testLCD = new tbsStruct();
    sysStru0 = new sysStruct();

    // 发送定时器初始化
    sendTim = new QTimer();
    sendTim->setInterval(SEND_INTERVAL);
    connect(sendTim, SIGNAL(timeout()), this, SLOT(sendCmdListFunc()));
    sendTim->start();
    this->tbsRepayInit();
}

Widget::~Widget()
{
    delete ui;
//    delete refreshData;
}


void Widget::on_selectFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open hex"), "/home/jana", tr("(*.hex)"));

    QFile file;
    if (fileName.isEmpty()) {
//        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        ui->lineEdit->setText("未选择文件");
    }
    else {
        ui->lineEdit->setText(fileName);
        if(hexFile.OpenHexFile(&file, fileName)) {
            hexFile.AllClear();
            QString errLog = hexFile.ReadHexFile(&file);
            QString HexStatus = QString("\
大小         = %1字节\n\
扩展线性地址 = 0x%2\n\
起始地址     = 0x%3\n\
解析HEX结果:\n%4")\
.arg(hexFile.hexLenth)\
.arg(hexFile.extendLinearAddress, 4, 16, QChar('0'))\
.arg(hexFile.address, 4, 16, QChar('0'))\
.arg(errLog);
            ui->label->setText(HexStatus);
        }

        file.close();
    }
}

void Widget::on_conectSerialButton_clicked()
{

}

void Widget::on_searchBtn_clicked()
{
    // serial se;
    se.RefreshSerial(ui);
}

void Widget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    static int count = 0;
    qDebug() << "=====================listWidget item click" << count;
    count++;
    this->SetTbsToTableAndChart(item, 0);
}

// 串口读取函数的定时器链接的函数，保证接收数据的完整
void Widget::ReadSerialTimeOut()
{
   sendTim->stop();
//    qDebug() << "in ++++ timeout     time"<< QTime::currentTime();
    readTim->stop();
    qDebug() << "=====串口接收定时器时间到";
    // 暂停tbs定时读取定时器防止报错

    se.TimeOut(ui, readTim);
    
    QString receiveDecode = dcode0.PlainTextDecode(ui);
    // 如果检测到某个结构体已经发生更新，则显示
    if(tverStru0->newDataStatus == true) {
        this->SetVersionLable();
        tverStru0->newDataStatus = false;
    }
    if(caliStru0->newDataStatus == true) {
        this->GetKB();
        caliStru0->newDataStatus = false;
    }
    if(sysStru0->newDataStatus == true) {
        ui->label_12->setText(sysStru0->displayData());
        if(ui->checkBox_3->checkState() == Qt::Checked) {
            QString fileName = ui->lineEdit_2->text();
            if(fileName != "") {
                csv::dataToCsv(fileName, sysStru0);
            }
        }
    }
    if(receiveDecode.contains("数据非法")) {
        sendTim->start();
        return;
    }
    int row = ui->listWidget->count();
    if(row == 0) {
        sendTim->start();
        return;
    }
    ui->listWidget->setCurrentRow(row - 1);
    this->SetTbsToTableAndChart(ui->listWidget->item(row - 1), 1);
    qDebug() << "======串口接收定时器函数结束";
//    qDebug() << "ou ++++ timeout     time"<< QTime::currentTime();
    // 开启tbs定时读取定时器
   sendTim->start();
}

// 解析listWidget内的数据，并写入到tableWidget中
void Widget::SetTbsToTableAndChart(QListWidgetItem *item, int flag)
{

    // 清除itemTable内的数据，并写入
    dcode0.clearTableItem(&itemTableList);

    // 判断listwidget最后一个item是否是tbs数据，是则写入到tbsUnit中
    qDebug() << "=================解析listwidget数据，写入到tablewidget中" << dcode0.ItemToTbs(item->text()) << flag;
    QTime currentTim = QTime::currentTime();
    if(dcode0.ItemToTbs(item->text())) {
        if(flag == 1) {
            chartV0->lineAddPoint("pack电压", currentTim,tbsStru0->dataCellList[0].uintVal);
            chartV0->lineAddPoint("电芯总压",  currentTim,tbsStru0->dataCellList[1].uintVal);

            chartV0->lineAddPoint("电芯1",    currentTim, tbsStru0->dataCellList[2].uintVal);
            chartV0->lineAddPoint("电芯2",    currentTim, tbsStru0->dataCellList[3].uintVal);
            chartV0->lineAddPoint("电芯3",    currentTim, tbsStru0->dataCellList[4].uintVal);
            chartV0->lineAddPoint("电芯4",    currentTim, tbsStru0->dataCellList[5].uintVal);

            chartV0->lineAddPoint("ntc1",    currentTim, tbsStru0->dataCellList[19].uintVal);
            chartV0->lineAddPoint("RM",      currentTim, tbsStru0->dataCellList[24].uintVal);
        }
        dcode0.itemToTable(&itemTableList);

        dcode0.SetStatusToBox(ui);
    }

}


void Widget::on_openBtn_clicked()
{
    qDebug() << "===================点击打开串口按钮";
    se.ClickOpenSerPort(ui);
    /* 创建接收数据信号槽 */
    connect(&se.SerialPort, &QSerialPort::readyRead, this, &Widget::SerialPortReadyRead_slot);

    // 创建延迟读取数据定时器
    readTim = new QTimer();
    readTim->setInterval(DELAY_READ_TIME);
    connect(readTim, SIGNAL(timeout()),this,SLOT(ReadSerialTimeOut()));

    if(ui->portStatus->text() == "串口已连接") {
        this->on_sendRegisterBox_clicked();
        ui->portStatus->setText("注册中");
    }
    if(ui->openBtn->text() == "打开串口") {
        if(ui->pushButton_3->text() == "停止读取tbs") {
            this->on_pushButton_3_clicked();
        }
    }

}

void Widget::SerialPortReadyRead_slot()
{
    qDebug() << "===============串口收到数据";
    se.ReadyRead(readTim);
}

void Widget::on_sendBox_clicked()
{
    QString sendData = ui->sendData->toPlainText();
    sendData = se.SerialSend(ui, sendData);

    qDebug() << "================点击串口数据发送，sendata = " << sendData;

    se.batComSendStatus = serial::COMPLETE;

    QString dcodeData = dcode0.AddTimeStamp(ui, sendData);

    ui->listWidget->addItem(dcodeData);
}

void Widget::on_clearReceiveDataButton_clicked()
{
    ui->receiveData->clear();
}

void Widget::SendAndDecode(QString sendData)
{
    sendData = se.SerialSend(ui, sendData);
    qDebug() << "==================发送数据函数sendata = " << sendData;
    // se.batComSendStatus = serial::COMPLETE;
    QString dcodeData = dcode0.AddTimeStamp(ui, sendData);
    dcode0.PlainTextDecode(ui);
}

void Widget::SendAndDecode(QByteArray sendArray)
{
    QString sendData = se.SerialSend(ui, sendArray);
    qDebug() << "==================发送数据函数sendata = " << sendData;
    // se.batComSendStatus = serial::COMPLETE;
    QString dcodeData = dcode0.AddTimeStamp(ui, sendData);
    dcode0.PlainTextDecode(ui);
}

void Widget::on_sendTbs_clicked()
{
    QString sendData = "00 00 04 01 13 55 AA 17";
    waitSendList.append(sendData);
}

void Widget::on_sendRegisterBox_clicked()
{
    QString sendData = "00 00 04 01 01 55 AA 05";
    waitSendList.append(sendData);
}

void Widget::sendCmdListFunc()
{
    QString tbsSendData = "00 00 04 01 13 55 AA 17";
//    SEND_TBS_COUNT
    static int tbsSendCount = 0;
    // qDebug()  << "in send cmdlistfunc time"<< QTime::currentTime() << "+++++++++tbsSendCount = " << tbsSendCount;
    if(hexSendList.size() <= 0 && byteSendList.size() <= 0) {
        tbsSendCount++;
        // 如果waitSendList有命令则发送最新入栈的cmd
        if(waitSendList.size() > 0) {
            readySendList.append(waitSendList.last());
            waitSendList.removeLast();
        } else {
        // 如果没有命令则发送tbs读取命令
            if (ui->pushButton_3->text() == "停止读取tbs") {
                if(ui->openBtn->text() == "打开串口") {
                    this->on_pushButton_3_clicked();
                    ui->pushButton_3->setEnabled(false);
                } else {
                    if(tbsSendCount >= SEND_TBS_COUNT) {
                        readySendList.append(tbsSendData);
                        tbsSendCount = 0;
                    }
                }
            }
        }
        if(readySendList.size() > 0) {
            SendAndDecode(readySendList.last());
            readySendList.removeLast();
        }
    } else {
        if(hexSendList.size() > 0) {
            // qDebug() << "hexsendlist send        "<< QTime::currentTime();
            SendAndDecode(hexSendList.first());
            hexSendList.removeFirst();
        }
    }
    // qDebug() << "ou send cmdlistfunc time"<< QTime::currentTime();
}

void Widget::sendHexListFunc()
{
//    QStringList
//    QByteArray sendArray = hexFile.n00dataArray.mid(hexFile.packetNum * hexFile.packetSize, hexFile.packetSize);

    if (ui->pushButton_3->text() == "停止读取tbs") {
        if(ui->openBtn->text() == "打开串口") {
            this->on_pushButton_3_clicked();
            ui->pushButton_3->setEnabled(false);
        }
    }

    if(hexSendList.size() > 0) {

    } else {

    }

    SendAndDecode(readySendList.last());
}
void Widget::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text() == "开始读取tbs") {

        ui->pushButton_3->setText("停止读取tbs");

    } else if (ui->pushButton_3->text() == "停止读取tbs") {
        ui->pushButton_3->setText("继续读取tbs");

    } else if (ui->pushButton_3->text() == "继续读取tbs") {
        ui->pushButton_3->setText("停止读取tbs");
        chartV0->ClearAllSeries();
    }

}

// 当在listWidget右击时跳入此函数
void Widget::on_PopupRightMenu(const QPoint& pos)
{
    QMenu* pMeue = new QMenu(this);

    QAction* pA1 = new QAction("复制",this);
    pMeue->addAction(pA1);

//    connect(pA1,&QAction::triggered,[=]{
//            qDebug()<<"1";
//        });
    QObject::connect(pA1, &QAction::triggered, [&]() {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(ui->listWidget->currentItem()->text());
        });

    QListWidgetItem* pItem = ui->listWidget->itemAt(pos);
    if(!pItem)
        return;
    pMeue->exec(QCursor::pos());
}

void Widget::on_clearReceiveDataButton_2_clicked()
{
    ui->listWidget->clear();
    chartV0->ClearAllSeries();
}





void Widget::on_pushButton_8_clicked()
{
    QString fileName = QString("%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HH_mm_ss"));
    QString saveFileUrl = QFileDialog::getSaveFileName(this,tr("Open hex"), "/home/" + fileName, tr("(*.csv)"));
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open hex"), "/home/jana", tr("(*.hex)"));
    if (saveFileUrl.isEmpty()) {
//        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        ui->lineEdit_2->setText("未选择文件");
    } else {
        ui->lineEdit_2->setText(saveFileUrl);
    }
}

void Widget::on_pushButton_7_clicked()
{
    QString fileName = ui->lineEdit_2->text();
    csv::tbsToCsv(ui, fileName, &dcode0);
}

void Widget::on_pushButton_2_clicked()
{
    dcode0.SetStatusToBox(ui);
}
// 读取csv的数据，并解析
void Widget::on_pushButton_clicked()
{
    QString saveFileUrl = QFileDialog::getOpenFileName(this, tr("Open csv"), "/home/", tr("(*.csv)"));
    QFile csvFile;
    if (saveFileUrl.isEmpty()) {
        ui->lineEdit_3->setText("未选择文件");
    } else {

        ui->lineEdit_3->setText(saveFileUrl);
        csv::ReadCsv(&csvFile, saveFileUrl);

        chartV0->ClearAllSeries();

        while(true) {
            QByteArray lineData = csvFile.readLine();
            if(lineData == nullptr) {
                break;
            }

            //去除item
            ui->listWidget->addItem(lineData);
            QListWidgetItem* item = ui->listWidget->item(ui->listWidget->count() - 1);
            //判断item->是否有效
            if(dcode0.ItemToTbs(item->text()) == false) {
                qDebug() << "csv itemtotbs false";
                // 删除无效item
                ui->listWidget->takeItem(ui->listWidget->count() - 1);
                continue;
            }
            // 获取text内的时间戳
            QString TimStr = dcode0.GetTime(item->text());


            // 移动光标到最新item
            int row = ui->listWidget->count();
            ui->listWidget->setCurrentRow(row - 1);

            // 添加从item中解析到的数据到tablewidget，label，listwidget中
            chartV0->lineAddPoint("pack电压", QTime::fromString(TimStr, "HH:mm:ss:zzz"),tbsStru0->dataCellList[0].uintVal);
            chartV0->lineAddPoint("电芯总压", QTime::fromString(TimStr, "HH:mm:ss:zzz"),tbsStru0->dataCellList[1].uintVal);

            chartV0->lineAddPoint("电芯1", QTime::fromString(TimStr, "HH:mm:ss:zzz"), tbsStru0->dataCellList[2].uintVal);
            chartV0->lineAddPoint("电芯2", QTime::fromString(TimStr, "HH:mm:ss:zzz"), tbsStru0->dataCellList[3].uintVal);
            chartV0->lineAddPoint("电芯3", QTime::fromString(TimStr, "HH:mm:ss:zzz"), tbsStru0->dataCellList[4].uintVal);
            chartV0->lineAddPoint("电芯4", QTime::fromString(TimStr, "HH:mm:ss:zzz"), tbsStru0->dataCellList[5].uintVal);

            chartV0->lineAddPoint("ntc1",  QTime::fromString(TimStr, "HH:mm:ss:zzz"), tbsStru0->dataCellList[19].uintVal);
            chartV0->lineAddPoint("RM",    QTime::fromString(TimStr, "HH:mm:ss:zzz"), tbsStru0->dataCellList[24].uintVal);

            dcode0.clearTableItem(&itemTableList);
            dcode0.itemToTable(&itemTableList);
            dcode0.SetStatusToBox(ui);
        }

    }
}

void Widget::on_pushButton_5_clicked()
{
   QWidget * refreshData=new QWidget();
   QLabel * refreshLabel = new QLabel(refreshData);
   refreshData->resize(800,800);
   refreshLabel->setText(
               "\
0.0.2更新内容\n\
电池状态颜色意义done\n\
电池序号 done\n\
电池状态信息显示改成中文 done\n\
串口连接信息显示done\n\
状态显示表优化 done\n\
代码中电芯名称修改 done\n\
状态信息顺序修正 done\n\
增加均衡状态 done\n\
0.0.3更新内容\n\
设置groupbox最大宽度改善ui\n\
改变串口函数读取模式\n\
解决状态显示错误问题（自加热，均衡，空闲）\n\
增加电池版本信息读取\n\
0.0.4更新内容\n\
打通kb校准流程ui\n\
0.0.5更新内容\n\
解决充放电管状态显示错误问题\n\
开关充放电管时暂停tbs读取，但好像还是会报错\n\
"
               );
   refreshData->show();
}

void Widget::on_getVersionButton_clicked()
{
    QString sendData = "00 00 04 01 16 55 AA 1A";
    waitSendList.append(sendData);
}
void Widget::SetVersionLable()
{
    QString output = "";
    foreach(QString key, tverStru0->keyList) {
        if(tverStru0->value(key).dataType == datTypDic::STRING) {
            output += tverStru0->value(key).valName + ":" + QString(tverStru0->value(key).byteArray);
        } else {
            output += tverStru0->value(key).valName + ":" + QString::number(tverStru0->value(key).uintVal);
        }
        output += "\n";
    }
    ui->versionLabel->setText(output);
}


QVector<QTableWidgetItem*> cliNameItem;
QVector<QTableWidgetItem*> valNameItem;
QVector<QTableWidgetItem*> newKItem;
QVector<QTableWidgetItem*> measureItem;
QVector<QTableWidgetItem*> exMeasureItem;
void Widget::GetKB()
{
    QString output = "";
    int idx = 0;
    foreach(QString key, caliStru0->keyList) {
//        qDebug() << key << idx << ;
        if((uint32_t)cliNameItem.size() < caliStru0->dataLenth) {
            cliNameItem.append(new QTableWidgetItem());
            valNameItem.append(new QTableWidgetItem());
            newKItem.append(new QTableWidgetItem());
            measureItem.append(new QTableWidgetItem());
            exMeasureItem.append(new QTableWidgetItem());
            ui->tableWidget_2->setItem(idx, 0, cliNameItem[idx]);
            ui->tableWidget_2->setItem(idx, 1, valNameItem[idx]);
            ui->tableWidget_2->setItem(idx, 2, newKItem[idx]);
            ui->tableWidget_2->setItem(idx, 3, measureItem[idx]);
            ui->tableWidget_2->setItem(idx, 4, exMeasureItem[idx]);
        }
        cliNameItem[idx]->setText(caliStru0->value(key)->valName);
        valNameItem[idx]->setText(QString::number(caliStru0->value(key)->uintVal));
        idx++;
    }
}

void Widget::on_getKB_clicked()
{
    QString sendData = "00 00 04 01 07 55 AA 0B";
    qDebug() << "================点击获取kb值按钮";
    waitSendList.append(sendData);
}

QByteArray IntToByte(uint32_t val, uint32_t lenth, datTypDic::ENDIAN_TYPE endianType) {
    QByteArray outPut;
    uint8_t byte = (uint8_t)(val >> 0 & 0xff);
    if(endianType == datTypDic::LITTLE) {
        for(int i = 0; i < (int)lenth; i++) {
            byte = (uint8_t)(val >> (i * 8) & 0xff);
            outPut.append(byte);
        }
    } else {
        for(int i = (int)lenth - 1; i >= 0; i--) {
            byte = (uint8_t)(val >> (i * 8) & 0xff);
            outPut.append(byte);
        }
    }
    return outPut;
}

void Widget::SetKb()
{

}

void Widget::on_pushButton_10_clicked()
{
    int idx = 0;
    bool ok;
    QByteArray sendArray, dataArray;
    qDebug() << "===============点击设置kb值按钮";
    sendArray += QByteArray(1, 0x00);
    if(newKItem.size() == 0) {
        ui->portStatus->setText("需要先读取KB值！！！");
        return;
    }
    foreach(QString key, caliStru0->keyList) {
        QByteArray newCellArray;
        if(newKItem[idx]->text() != "") {
            uint16_t newKValue = newKItem[idx]->text().toUInt(&ok, 10);
            dataArray += IntToByte(newKValue, caliStru0->value(key)->typeLenth, caliStru0->value(key)->endianType);
        } else {
            dataArray += caliStru0->value(key)->byteArray;
        }
        idx++;
    }
    sendArray += IntToByte(dataArray.size() + 4, 2, datTypDic::BIG); // 加4是单板类型到握手字2的长度
    sendArray.push_back(0x01); // 单板类型
    sendArray.push_back(0x08); // 命令类型
    sendArray.push_back(0x55); // 握手字1
    sendArray.push_back(0xAA); // 握手字2
    sendArray += dataArray;

    QByteArray checkArray = sendArray.mid(1, -1);
    uint8_t checkSum = 0;
    foreach(uint8_t val, checkArray) {
        checkSum += val;
    }
    sendArray.push_back(checkSum);

    QString sendStr;

    foreach(uint8_t val, sendArray) {
        sendStr += QString("%1").arg(val, 2, 16, QChar('0')) + ' ';
    }

    waitSendList.append(sendStr);
}

void Widget::on_pushButton_6_clicked()
{
    if(tbsStru0->dataCellList[0].uintVal == 0) {
        ui->portStatus->setText("没有读取电压！！！");
        return;
    }
    if(newKItem.size() == 0) {
        ui->portStatus->setText("需要先读取k值！！！");
        return;
    }
    for(uint8_t i = 0; i < 18; i++) {
        measureItem[i]->setText(QString::number(tbsStru0->dataCellList[i].uintVal,10));
    }
}

void Widget::on_pushButton_9_clicked()
{
    bool ok = true;
    if(exMeasureItem.size() == 0 || measureItem.size() == 0) {
        ui->portStatus->setText("没有电压！！！");
        return;
    }
    for(uint8_t i = 0; i < 18; i++) {
        if(exMeasureItem[i]->text() != "" && measureItem[i]->text() != "") {
            uint64_t newKB = exMeasureItem[i]->text().toInt(&ok, 10) * 10000 / measureItem[i]->text().toInt(&ok, 10);
            newKItem[i]->setText(QString::number((newKB * valNameItem[i]->text().toInt(&ok, 10) / 10000), 10));
        }
    }
}

void Widget::on_openChgFet_clicked()
{
    QString sendData = "00 00 04 01 0A 55 AA 0E";
    waitSendList.append(sendData);
}

void Widget::on_closeChgFet_clicked()
{
    QString sendData = "00 00 04 01 0B 55 AA 0F";
    waitSendList.append(sendData);
}

void Widget::on_openDisFet_clicked()
{
    QString sendData = "00 00 04 01 0C 55 AA 10";
    waitSendList.append(sendData);
}

void Widget::on_closeDisFet_clicked()
{
    QString sendData = "00 00 04 01 0D 55 AA 11";
    waitSendList.append(sendData);
}
void Widget::on_pushButton_4_clicked()
{
//    QString sendData = "00 00 04 01 53 55 AA 57";
//    waitSendList.append(sendData);
    hexFile.DownloadClear();

    if(hexFile.beginDownloadState == 0) {
        hexFile.beginDownloadState = 1;
    }
    if(hexFile.shakeSuccessTime < 3) {
        QString writeStr = hexFile.packetToSendString(hexDecode::ENTER_BOOTMODE);
        hexSendList.append(writeStr);
        return;
    }
    // if(hexFile.writeSuccessTime < (hexFile.lenth / hexFile.packetSize)) {
    //     QString writeStr = hexFile.packetToSendString(hexDecode::WRITE_FLASH);
    //     hexSendList.append(writeStr);
    //     return;
    // }
}
void Widget::on_pushButton_11_clicked()
{
    // 读取ic
    QString sendData = "00 00 04 01 51 55 AA 55";
    waitSendList.append(sendData);

}

QMap<QString, QCheckBox*> checkBMap;
QMap<QString, QCheckBox*> sysCheckBMap;
void Widget::on_pushButton_12_clicked()
{

}
void Widget::tbsRepayInit()
{
    testLCD->addStatusBits();
    uint32_t x = 0,y = 0;
    foreach(dataCell cell,testLCD->dataCellList) {
        if (cell.valName.contains("HEX")) {
            foreach(QString bitName, testLCD->value(cell.valName)->bitMap.values())
            {
                QCheckBox* checkbox = new QCheckBox();
                checkbox->setText(bitName);
                ui->gridLayout_21->addWidget(checkbox, x, y);
                checkBMap.insert(bitName, checkbox);
                x++;
            }
            x = 0;
            y++;
        }
    }

    foreach(dataCell cell,sysStru0->dataCellList) {
        if (cell.valName.contains("HEX")) {
            foreach(QString bitName, testLCD->value(cell.valName)->bitMap.values())
            {
                QCheckBox* checkbox = new QCheckBox();
                checkbox->setText(bitName);
                ui->gridLayout_23->addWidget(checkbox, x, y);
                sysCheckBMap.insert(bitName, checkbox);
                x++;
            }
            x = 0;
            y++;
        }
    }
}


void Widget::on_tbsReply_clicked()
{
    QByteArray dataArray, sendDataArray;

    if(checkBMap["空闲"]->checkState() == Qt::Checked) {
        checkBMap["充电"]->setCheckState(Qt::Unchecked);
        checkBMap["放电"]->setCheckState(Qt::Unchecked);
        checkBMap["满充"]->setCheckState(Qt::Unchecked);
    }
    foreach(dataCell cell,testLCD->dataCellList) {
        testLCD->value(cell.valName)->uintVal = 0;
        if (cell.valName.contains("HEX")) {
            // 遍历名字带有HEX的变量的bit
            foreach(QString bitName, testLCD->value(cell.valName)->bitMap.values())
            {
                // 如果这个bit的checkBox打勾
                if(checkBMap.value(bitName)->checkState() == Qt::Checked) {
                    testLCD->value(cell.valName)->uintVal |= testLCD->value(cell.valName)->findBitVal(bitName);
                }
            }
            qDebug() << "uintVal = 0x" << QString::number(testLCD->value(cell.valName)->uintVal,16);
        }
    }
    waitSendList.append(testLCD->OutPutStru());

//    qDebug() << "utf8Buffer size = " << testLCD->OutPutStru().size();

}

void Widget::on_checkBox_2_stateChanged(int arg1)
{
    qDebug() << arg1 << "arg1";

    foreach(QCheckBox* box, checkBMap) {
        box->setCheckState((Qt::CheckState)arg1);
    }
}

void Widget::on_pushButton_13_clicked()
{
    QByteArray dataArray, sendDataArray;

    foreach(dataCell cell,sysStru0->dataCellList) {
        sysStru0->value(cell.valName)->uintVal = 0;
        if (cell.valName.contains("HEX")) {
            // 遍历名字带有HEX的变量的bit
            foreach(QString bitName, testLCD->value(cell.valName)->bitMap.values())
            {
                // 如果这个bit的checkBox打勾
                if(sysCheckBMap.value(bitName)->checkState() == Qt::Checked) {
                    sysStru0->value(cell.valName)->uintVal |= sysStru0->value(cell.valName)->findBitVal(bitName);
                }
            }
            qDebug() << "uintVal = 0x" << QString::number(sysStru0->value(cell.valName)->uintVal,16);
        }
    }
    sysStru0->value("系统电池数量")->uintVal = ui->comboBox->currentIndex() + 1;
    sysStru0->value("系统SOC值")->uintVal = 50;
    waitSendList.append(sysStru0->OutPutStru());
}
