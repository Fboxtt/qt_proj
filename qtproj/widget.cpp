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


serial se;
textDcode dcode0;
QVector<QTableWidgetItem> itemTableList(50);

chartV* chartV0;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    se.Init(ui);
    ui->sendBox->setEnabled(false);


    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu); // 设置右键菜单
    connect(ui->listWidget,&QListWidget::customContextMenuRequested,this,&Widget::on_PopupRightMenu); // 链接右击和on_PopupRightMenu槽函数，

    // listWidget设置
    ui->listWidget->setWordWrap(true); // 设置可以换行 listwidget格式设置
    ui->listWidget->setStyleSheet("QListWidget{font-size:10px;}"); // 设置字体大小 listwidget格式设置

    // tableWidget设置
    ui->tableWidget->setFont(QFont("黑体", 5)); // table字体设置
    for(int idx = 0, limit = itemTableList.size(); idx < limit; idx++) {
        itemTableList[idx].setText(QString("%1").arg(idx,0,10));
        ui->tableWidget->setItem(idx / 3, idx % 3, &itemTableList[idx]);
    }

    // 波形chart配置
    chartV0 = new chartV();
    chartV0->addNewChart(ui->tab_2,"总电压", "pack电压", "mV");
    chartV0->addNewChart(ui->tab_2,"电芯电压", "电芯电压", "mV");
    chartV0->addNewChart(ui->tab_2,"温度", "ntc1", "C");

    ui->gridLayout_5->addWidget(chartV0->chartMap.value("总电压")->chartview, 0, 0);
    ui->gridLayout_5->addWidget(chartV0->chartMap.value("电芯电压")->chartview, 0, 1);
    ui->gridLayout_5->addWidget(chartV0->chartMap.value("温度")->chartview, 1, 0);
//    chartV0->addNewChart(ui->tab_2, "", "", "");


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_selectFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open hex"), "/home/jana", tr("(*.hex)"));
    if (fileName.isEmpty()) {
//        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        ui->lineEdit->setText("未选择文件");
    }
    else {
        ui->lineEdit->setText(fileName);
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

void Widget::ReceveHexDecode()
{
    QString receiveDecode = dcode0.DecodeHexToCommand(ui);
    ui->listWidget->addItem(receiveDecode);

    int row = ui->listWidget->count();  
    // 清除itemTable内的数据
    dcode0.clearTableItem(&itemTableList);
    // 判断listwidget最后一个item是否是tbs数据，是则写入到tbsUnit中
//    bool stat = this->on_listWidget_itemClicked(ui->listWidget->item(row - 1));
    bool stat = dcode0.ItemToTbs(ui->listWidget->item(row - 1));
    if(stat == true) {
        dcode0.itemToTable(&itemTableList);

        chartV0->lineAddPoint("pack电压", (*dcode0.tbsUnion)[0].uintVal);
        chartV0->lineAddPoint("电芯电压", (*dcode0.tbsUnion)[2].uintVal);
        chartV0->lineAddPoint("ntc1", (*dcode0.tbsUnion)[19].uintVal);


    }
}

void Widget::onTimeOut()
{
    se.TimeOut(tim);
    this->ReceveHexDecode();
}

void Widget::on_openBtn_clicked()
{
    qDebug() << "open serial btn";
    se.ClickOpenSerPort(ui);
    /* 创建接收数据信号槽 */
    connect(&se.SerialPort, &QSerialPort::readyRead, this, &Widget::SerialPortReadyRead_slot);

    tim = new QTimer();
    tim->setInterval(100);
    connect(tim, SIGNAL(timeout()),this,SLOT(onTimeOut()));
}

void Widget::SerialPortReadyRead_slot()
{
    qDebug() << "in ready slot";
    se.serial_Read(ui,tim);
}

void Widget::on_sendBox_clicked()
{
    QString sendData = ui->sendData->toPlainText();
    sendData = se.on_sendBox_clicked(ui, sendData);

    qDebug() << "sendata = " << sendData;

    se.batComSendStatus = serial::COMPLETE;

    QString dcodeData = dcode0.SendDataDecode(ui, sendData);

    ui->listWidget->addItem(dcodeData);
}

void Widget::on_clearReceiveDataButton_clicked()
{
    ui->receiveData->clear();
}
void Widget::SendAndDecode(QString sendData)
{
    sendData = se.on_sendBox_clicked(ui, sendData);
    qDebug() << "sendata = " << sendData;
    se.batComSendStatus = serial::COMPLETE;
    QString dcodeData = dcode0.SendDataDecode(ui, sendData);
    ui->listWidget->addItem(dcodeData);
}
void Widget::on_sendTbs_clicked()
{
    QString sendData = "00 00 65 01 93 55 AA 00 13 33 00\
 00 5F 33 00 00 DA 0C D8 0C D8\
 0C D5 0C 00 00 00 00 00 00 00\
 00 00 00 00 00 00 00 00 00 00\
 00 00 00 00 00 00 00 00 00 00\
 00 18 00 18 00 00 00 00 00 00\
 00 AE 0F 40 1F 00 00 00 00 00\
 00 00 00 00 00 00 00 00 00 00\
 00 00 00 00 00 00 00 00 00 32\
 00 50 00 00 00 00 00 00 00 00\
 00 00 00 2D";
    this->SendAndDecode(sendData);
}

void Widget::on_sendRegisterBox_clicked()
{
    QString sendData = "00 00 05 01 81 55 AA 06 8C";
    this->SendAndDecode(sendData);
}

void Widget::sendCmdRecieveWave()
{
    QString sendData = "00 00 04 01 13 55 AA 17";
    this->SendAndDecode(sendData);
}

void Widget::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text() == "开始读取tbs") {
        tbsTim = new QTimer();
        tbsTim->setInterval(1000);
        connect(tbsTim, SIGNAL(timeout()), this, SLOT(sendCmdRecieveWave()));
        tbsTim->start();
        ui->pushButton_3->setText("停止读取tbs");

    } else if (ui->pushButton_3->text() == "停止读取tbs") {
        tbsTim->stop();
        ui->pushButton_3->setText("继续读取tbs");

    } else if (ui->pushButton_3->text() == "继续读取tbs") {
        tbsTim->start();
        ui->pushButton_3->setText("停止读取tbs");
    }

}


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
}

bool Widget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    return dcode0.ItemToTbs(item);
} 

void Widget::on_pushButton_4_clicked()
{
//    delete(tableItem);
}

void Widget::on_pushButton_8_clicked()
{
    QString fileName = QString("%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HH_mm_ss"));
    QString saveFileUrl = QFileDialog::getSaveFileName(this,tr("Open hex"), "/home/" + fileName, tr("(*.csv)"));
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open hex"), "/home/jana", tr("(*.hex)"));
    if (saveFileUrl.isEmpty()) {
//        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
        ui->lineEdit_2->setText("未选择文件");
    }
    else {
        ui->lineEdit_2->setText(saveFileUrl);
    }
}

void Widget::on_pushButton_7_clicked()
{
    QString fileName = ui->lineEdit_2->text();
    csv::tbsToCsv(ui, fileName, &dcode0);
}

void Widget::on_pushButton_clicked()
{
//    QPixmap greenPic = QPixmap("://images/greenpot.png");
//    greenPic = greenPic.scaled(510,40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    QPixmap redPic = QPixmap("://images/greenpot.png");
    int width = 200;
    ui->batGroupBox->setMinimumSize(width, 1000);
    ui->batGroupBox->setMaximumWidth(width);

    QPixmap pixmap = QPixmap(width,30);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    painter.setBrush(Qt::green);
    painter.drawEllipse(width - 45 - 2*30, 0, 30, 30);
    painter.setBrush(Qt::red);
    painter.drawEllipse(width - 35 - 30,   0, 30, 30);
    painter.drawText(0,0, 150, 30, Qt::AlignLeft | Qt::AlignVCenter,"电芯过压");
    ui->label->setPixmap(pixmap);
//    ui->label->setText("test");
}

void Widget::on_pushButton_2_clicked()
{
    dcode0.SetStatusToGBox(ui->gridLayout_7);
}
