#include "chart.h"

line::line(){

}

chart::chart(QWidget *parent)
{
    // 初始化chart窗口
    this->chartview = new QChartView(parent);
    this->qchart = new QChart();
    this->chartview->setChart(this->qchart);
    this->chartview->resize(1000, 1000);

    // chart改名
    this->qchart->setTitle("电池状态折线表");

    // 初始化X轴
    this->axisX = new QValueAxis;
    axisX->setRange(0, 5);
    axisX->setTitleText("time(secs)");
}


void chart::addNewLine(QString lineName, QString axisYName)
{
    if(lineMap.contains(lineName))
    {
        qDebug() << "已有目标曲线" << lineName;
        return;
    }
    // map内添加新键对
    line *line1 = new line();
    lineMap.insert(lineName, line1);


    //  新series添加到chart
    line1->series = new QLineSeries();
    line1->series->setName(lineName);
    this->qchart->addSeries(line1->series);

    // 定义新Y轴
    line1->axisY = new QValueAxis;
    line1->axisY->setRange(-5, 5);
    line1->axisY->setTitleText(axisYName);

    // 新Y轴和series和chart联系起来
    this->qchart->addAxis ( this->axisX, Qt::AlignBottom );
    this->qchart->addAxis ( line1->axisY, Qt::AlignLeft );
    line1->series->attachAxis ( this->axisX );
    line1->series->attachAxis ( line1->axisY );
}

void chart::addNewPoint(QString lineName, int newY)
{
    static int x = 0, minY = -5, maxY = 5;
    if(!lineMap.contains(lineName))
    {
        qDebug() << "没有目标曲线无法添加";
        return;
    }

    // 在map中使用名字键找到对象
    line *line1 = lineMap.value(lineName);

    *(line1->series) << QPointF(x,newY);

    // 求出曲线极值，并修改Y轴，以适应窗口大小
    if(newY != maxY) {
        maxY = maxY > newY ? maxY : newY;
        minY = minY < newY ? minY : newY;
        line1->axisY->setRange(minY, maxY);
    }

    this->axisX->setRange(0, x);
    x++;
}


