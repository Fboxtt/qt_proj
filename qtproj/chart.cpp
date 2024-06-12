#include "chart.h"

line::line(){

}

chart::chart(QWidget *parent)
{
    chartview = new QChartView(parent);
    qchart = new QChart();
    chartview->setChart(qchart);
    chartview->resize(1000,1000);



//    series0 = new QLineSeries();
//    series0->setName(QStringLiteral("电压曲线"));
//    *series0 << QPointF(1,5) << QPointF(2,1) << QPointF(3, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7);
//    qchart->addSeries(series0);


    this->axisX = new QValueAxis;
//    axisY = new QValueAxis;
//    setAxis(0, 10, -5, 10);
//    qchart->setAxisX(axisX, series0);//为序列添加坐标轴
//    qchart->setAxisY(axisY);
    axisX->setRange(0,5);
    axisX->setTitleText("time(secs)");
}

void chart::addPoint(int x, int y)
{
    *series0 << QPointF(x, y);
}

void chart::setAxis(int minX, int maxX, int minY, int maxY)
{

    axisX->setRange(minX,maxX);
    axisX->setTitleText("time(secs)");

    axisY->setRange(minY,maxY);
    axisY->setTitleText("value");
}

void chart::addNewLine(QString lineName, QString axisYName)
{
    if(lineMap.contains(lineName))
    {
        qDebug() << "已有目标曲线" << lineName;
        return;
    }

    line *line1 = new line();
    lineMap.insert(lineName, line1);

    line1->series = new QLineSeries();
    qchart->addSeries(line1->series);

    line1->axisY = new QValueAxis;
    line1->axisY->setRange(-5, 5);
    line1->axisY->setTitleText(axisYName);
//    qchart->setAxisY(line1->axisY, line1->series);
//    qchart->setAxisX(line1->axisY, line1->series);

    qchart->addAxis ( this->axisX, Qt::AlignBottom );
    qchart->addAxis ( line1->axisY, Qt::AlignLeft );
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
    line *line1 = lineMap.value(lineName);

    *(line1->series) << QPointF(x,newY);

    if(newY != maxY) {
        maxY = maxY > newY ? maxY : newY;
        minY = minY < newY ? minY : newY;
        line1->axisY->setRange(minY, maxY);
    }

    this->axisX->setRange(0, x);
    x++;
}
