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
void chartV::lineAddPoint(QString lineName, int newY)
{
    QLineSeries* series = nullptr;
    chart* chDest = nullptr;
    static int x = 0;
    static QTime startTim = QTime::currentTime();

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

    x = startTim.secsTo(QTime::currentTime());
    *(series) << QPointF(x,newY);

    // 求出曲线极值，让Y轴随着Y极值变化，形成活动窗口
    if(newY != chDest->maxY) {
        chDest->maxY = chDest->maxY > newY ? chDest->maxY : newY;
        chDest->minY = chDest->minY < newY ? chDest->minY : newY;
        chDest->axisY->setRange(chDest->minY, chDest->maxY);
    }

    chDest->axisX->setRange(0, x);
    qDebug() << "x = " << startTim.secsTo(QTime::currentTime()) << "newy = " << newY;
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

void chart::addNewPoint(QString lineName, int newY)
{
    static int x = 0, minY = -5, maxY = 5;
    if(!seriesMap.contains(lineName))
    {
        qDebug() << "没有目标曲线无法添加";
        return;
    }

    // 在map中使用名字键找到对象
    QLineSeries *series = seriesMap.value(lineName);

    *(series) << QPointF(x,newY);

    // 求出曲线极值，让Y轴随着Y极值变化，形成活动窗口
    if(newY != maxY) {
        maxY = maxY > newY ? maxY : newY;
        minY = minY < newY ? minY : newY;
        this->axisY->setRange(minY, maxY);
    }

    this->axisX->setRange(0, x);
    x++;
}


