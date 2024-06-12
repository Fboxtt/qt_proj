#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QListWidgetItem>
#include <QChartView>
#include <Qtcharts>
#include <QLineSeries>

class line
{
public:
    line();
    QValueAxis *axisY;
    QLineSeries *series;
};

class chart
{
public:
    chart(QWidget *parent = nullptr);
    QChartView *chartview;
    QChart *qchart;

    QLineSeries *series0;
    void addPoint(int x, int y);

    QValueAxis *axisX;
    QValueAxis *axisY;
    QValueAxis *axisY2;
    void setAxis(int minX, int maxX, int minY, int maxY);


    QMap<QString, line*> lineMap;
    void addNewLine(QString lineName, QString axisYName);
    void addNewPoint(QString lineName, int y);
};

#endif // CHART_H
