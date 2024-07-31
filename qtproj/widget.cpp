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


//     数据结构体初始化函数
    tverStru0 = new tverStruct();
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
            hexFile.Clear();
            QString errLog = hexFile.ReadHexFile(&file);
            QString HexStatus = QString("\
大小         = %1字节\n\
扩展线性地址 = 0x%2\n\
起始地址     = 0x%3\n\
解析HEX结果:\n%4")\
.arg(hexFile.lenth)\
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
    qDebug() << "listWidget item click" << count;
    count++;
    this->SetTbsToTableAndChart(item, 0);
}

// 串口读取函数的定时器链接的函数，保证接收数据的完整
void Widget::ReadSerialTimeOut()
{
    se.TimeOut(ui, tim);

    QString receiveDecode = dcode0.PlainTextDecode(ui);
    // 如果检测到某个结构体已经发生更新，则显示
    if(tverStru0->newDataStatus == true) {
        this->SetVersionLable();
    }

    if(receiveDecode.contains("数据非法")) {
        return;
    }
    int row = ui->listWidget->count();
    if(row == 0) {
        return;
    }
    ui->listWidget->setCurrentRow(row - 1);
    this->SetTbsToTableAndChart(ui->listWidget->item(row - 1), 1);
}

// 解析listWidget内的数据，并写入到tableWidget中
void Widget::SetTbsToTableAndChart(QListWidgetItem *item, int flag)
{

    // 清除itemTable内的数据，并写入
    dcode0.clearTableItem(&itemTableList);

    // 判断listwidget最后一个item是否是tbs数据，是则写入到tbsUnit中
    qDebug() << "=================" << dcode0.ItemToTbs(item->text()) << flag;
    QTime currentTim = QTime::currentTime();
    if(dcode0.ItemToTbs(item->text())) {
        if(flag == 1) {
            chartV0->lineAddPoint("pack电压", currentTim,(*dcode0.tbsUnion)[0].uintVal);
            chartV0->lineAddPoint("电芯总压",  currentTim,(*dcode0.tbsUnion)[1].uintVal);

            chartV0->lineAddPoint("电芯1",    currentTim, (*dcode0.tbsUnion)[2].uintVal);
            chartV0->lineAddPoint("电芯2",    currentTim, (*dcode0.tbsUnion)[3].uintVal);
            chartV0->lineAddPoint("电芯3",    currentTim, (*dcode0.tbsUnion)[4].uintVal);
            chartV0->lineAddPoint("电芯4",    currentTim, (*dcode0.tbsUnion)[5].uintVal);

            chartV0->lineAddPoint("ntc1",    currentTim, (*dcode0.tbsUnion)[19].uintVal);
            chartV0->lineAddPoint("RM",      currentTim, (*dcode0.tbsUnion)[24].uintVal);
        }
        dcode0.itemToTable(&itemTableList);

        dcode0.SetStatusToBox(ui);
    }

}


void Widget::on_openBtn_clicked()
{
    qDebug() << "open serial btn";
    se.ClickOpenSerPort(ui);
    /* 创建接收数据信号槽 */
    connect(&se.SerialPort, &QSerialPort::readyRead, this, &Widget::SerialPortReadyRead_slot);

    tim = new QTimer();
    tim->setInterval(400);
    connect(tim, SIGNAL(timeout()),this,SLOT(ReadSerialTimeOut()));
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
    qDebug() << "in ready slot";
    se.ReadyRead(tim);
}

void Widget::on_sendBox_clicked()
{
    QString sendData = ui->sendData->toPlainText();
    sendData = se.SerialSend(ui, sendData);

    qDebug() << "sendata = " << sendData;

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
    qDebug() << "sendata = " << sendData;
    // se.batComSendStatus = serial::COMPLETE;
    QString dcodeData = dcode0.AddTimeStamp(ui, sendData);
    dcode0.PlainTextDecode(ui);
}
void Widget::on_sendTbs_clicked()
{
//     QString sendData = "00 00 65 01 93 55 AA 00 13 33 00\
//  00 5F 33 00 00 DA 0C D8 0C D8\
//  0C D5 0C 00 00 00 00 00 00 00\
//  00 00 00 00 00 00 00 00 00 00\
//  00 00 00 00 00 00 00 00 00 00\
//  00 18 00 18 00 00 00 00 00 00\
//  00 AE 0F 40 1F 00 00 00 00 00\
//  00 00 00 00 00 00 00 00 00 00\
//  00 00 00 00 00 00 00 00 00 32\
//  00 50 00 00 00 00 00 00 00 00\
//  00 00 00 2D";
    QString sendData = "00 00 04 01 13 55 AA 17";
    this->SendAndDecode(sendData);
}

void Widget::on_sendRegisterBox_clicked()
{
    QString sendData = "00 00 04 01 01 55 AA 05";
    this->SendAndDecode(sendData);
}

void Widget::sendCmdRecieveWave()
{
    QString sendData = "00 00 04 01 13 55 AA 17";
    // 如果串口处于关闭状态，并且tbs正在读取中，则停止tbs读取
    if(ui->openBtn->text() == "打开串口") {
        if (ui->pushButton_3->text() == "停止读取tbs") {
            this->on_pushButton_3_clicked();
            ui->pushButton_3->setEnabled(false);
            tbsTim->stop();
        }
        return;
    }
    this->SendAndDecode(sendData);
}

void Widget::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text() == "开始读取tbs") {
        if(tbsTim == nullptr) {
            tbsTim = new QTimer();
            tbsTim->setInterval(1000);
            connect(tbsTim, SIGNAL(timeout()), this, SLOT(sendCmdRecieveWave()));
        }
        chartV0->ClearAllSeries();
        tbsTim->start();
        ui->pushButton_3->setText("停止读取tbs");

    } else if (ui->pushButton_3->text() == "停止读取tbs") {
        tbsTim->stop();
        ui->pushButton_3->setText("继续读取tbs");

    } else if (ui->pushButton_3->text() == "继续读取tbs") {
        tbsTim->start();
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



void Widget::on_pushButton_4_clicked()
{
    if(hexFile.exist == true && ui->openBtn->text() == "关闭串口") {
//        this->SendAndDecode();
        se.SerialSend(ui,hexFile.n00dataArray);
    } else {
        qDebug() << "hex文件err，或者串口未打开";
    }
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
            chartV0->lineAddPoint("pack电压", QTime::fromString(TimStr, "HH:mm:ss:zzz"),(*dcode0.tbsUnion)[0].uintVal);
            chartV0->lineAddPoint("电芯总压", QTime::fromString(TimStr, "HH:mm:ss:zzz"),(*dcode0.tbsUnion)[1].uintVal);

            chartV0->lineAddPoint("电芯1", QTime::fromString(TimStr, "HH:mm:ss:zzz"), (*dcode0.tbsUnion)[2].uintVal);
            chartV0->lineAddPoint("电芯2", QTime::fromString(TimStr, "HH:mm:ss:zzz"), (*dcode0.tbsUnion)[3].uintVal);
            chartV0->lineAddPoint("电芯3", QTime::fromString(TimStr, "HH:mm:ss:zzz"), (*dcode0.tbsUnion)[4].uintVal);
            chartV0->lineAddPoint("电芯4", QTime::fromString(TimStr, "HH:mm:ss:zzz"), (*dcode0.tbsUnion)[5].uintVal);

            chartV0->lineAddPoint("ntc1",  QTime::fromString(TimStr, "HH:mm:ss:zzz"), (*dcode0.tbsUnion)[19].uintVal);
            chartV0->lineAddPoint("RM",    QTime::fromString(TimStr, "HH:mm:ss:zzz"), (*dcode0.tbsUnion)[24].uintVal);

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
   refreshLabel->setText("\
0.0.2更新内容\n\
电池状态颜色意义done\n\
电池序号 done\n\
电池状态信息显示改成中文 done\n\
串口连接信息显示done\n\
状态显示表优化 done\n\
代码中电芯名称修改 done\n\
状态信息顺序修正 done\n\
增加均衡状态 done\n"
               );
   refreshData->show();
}

void Widget::on_getVersionButton_clicked()
{
    QString sendData = "00 00 04 01 16 55 AA 1A";
    this->SendAndDecode(sendData);
//    connect(ui->listWidget,&QListWidget::customContextMenuRequested,this,&Widget::SetVersionLable);
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
