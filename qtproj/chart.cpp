#include "chart.h"

chart::chart(QWidget *parent, QString chartName)
{
    // 初始化chart窗口
    this->chartview = new QChartView(parent);
    this->qchart = new QChart();
    this->chartview->setChart(this->qchart);
    this->chartview->resize(1000, 1000);

    // chart改名
    this->qchart->setTitle(chartName);

    // 定义Y轴和X轴
    this->axisY = new QValueAxis;
    this->axisX = new QValueAxis;
    axisY->setRange(-5, 5);
    axisX->setRange(0, 5);


}

chartV::chartV()
{

}

void chartV::addNewChart(QWidget *parent, QString chartName, QString lineName, QString axisYName)
{
    chart* chart1;
    if(chartMap.contains(chartName)) {
        qDebug() << chartName << "这个chart已经存在";
        chart1 = chartMap.value(chartName);
    } else {
        chart1 = new chart(parent, chartName);
        chartMap.insert(chartName, chart1);
    }

    chart1->addNewLine(lineName, axisYName);
}
void chartV::lineAddPoint(QString lineName, QTime inputTim, int newY)
{
    QLineSeries* series = nullptr;
    chart* chDest = nullptr;
    static int x = 0;
//    static QTime FirstPTim = QTime::currentTime();

    foreach(chart* ch0, this->chartMap.values()) {
        if(ch0->seriesMap.contains(lineName)) {
            chDest = ch0;
            series = chDest->seriesMap.value(lineName);
        }
    }

    if(series == nullptr) {
        qDebug() << "not this series" << lineName;
        return;
    }

    if(series->count() == 0){
        chDest->firstTim = inputTim;
    }
//    qDebug() << "2 fist time" << chDest->firstTim.toString() << "inputTim" << inputTim.toString();
    x = chDest->firstTim.secsTo(inputTim);
    *(series) << QPointF(x,newY);

    // 求出曲线极值，让Y轴随着Y极值变化，形成活动窗口
    if(newY != chDest->maxY) {
        chDest->maxY = chDest->maxY > newY ? chDest->maxY : newY;
        chDest->minY = chDest->minY < newY ? chDest->minY : newY;
        // *1.33是为了让波形图居中偏上
        chDest->axisY->setRange(chDest->minY, chDest->maxY * 1.33);
    }

    chDest->axisX->setRange(0, x);
    qDebug() << "x = " << x << "newy = " << newY;
}

void chartV::lineClearPoint(QString lineName)
{
    QLineSeries* series = nullptr;
    chart* chDest = nullptr;

    foreach(chart* ch0, this->chartMap.values()) {
        if(ch0->seriesMap.contains(lineName)) {
            chDest = ch0;
            series = ch0->seriesMap.value(lineName);
        }
    }

    if(series == nullptr) {
        qDebug() << "not this series" << lineName;
        return;
    }

    series->clear();

    chDest->maxY = 5;
    chDest->minY = 0;
    // 求出曲线极值，让Y轴随着Y极值变化，形成活动窗口

    chDest->axisX->setRange(0, 1);
//    qDebug() << "x = " << inputTim.secsTo(QTime::currentTime()) << "newy = " << newY;
}
void chartV::ClearAllSeries(void)
{

    foreach(chart* ch0, this->chartMap.values()) {
        foreach(QLineSeries* series, ch0->seriesMap.values()) {
            if(series == nullptr) {
                qDebug() << "not this series";
                continue;
            }
            series->clear();

            ch0->maxY = 5;
            ch0->minY = 0;
            // 求出曲线极值，让Y轴随着Y极值变化，形成活动窗口
            ch0->axisX->setRange(0, 1);
        }
    }
}
void chart::addNewLine(QString lineName, QString axisYName)
{
    if(seriesMap.contains(lineName))
    {
        qDebug() << "已有目标曲线" << lineName;
        return;
    }
    // map内添加新键对
    QLineSeries *series = new QLineSeries();
    seriesMap.insert(lineName, series);


    //  新series添加到chart
//    series = new QLineSeries();
    series->setName(lineName);
    this->qchart->addSeries(series);




    // 新Y轴和series和chart联系起来
    axisY->setTitleText(axisYName);
    this->qchart->addAxis ( this->axisY, Qt::AlignLeft );
    series->attachAxis ( this->axisY );

    // 初始化X轴
    axisX->setTitleText("time(secs)");
    this->qchart->addAxis ( this->axisX, Qt::AlignBottom );
    series->attachAxis ( this->axisX );
}



