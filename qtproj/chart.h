#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QListWidgetItem>
#include <QChartView>
#include <Qtcharts>
#include <QLineSeries>

class chart
{
public:
    chart(QWidget *parent, QString chartName);
    QChartView *chartview;
    QChart *qchart;
    QValueAxis *axisX;
    QValueAxis *axisY;

    int maxY = 5;
    int minY = 0;
    int maxX;
    QMap<QString, QLineSeries*> seriesMap;
    void addNewLine(QString lineName, QString axisYName);
    void addNewPoint(QString lineName, int y);
};

class chartV
{
public:
    chartV();

    QMap<QString, chart*> chartMap;
    void addNewChart(QWidget *parent, QString chartName, QString lineName, QString axisYName);
    void lineAddPoint(QString lineName, int newY);
};

#endif // CHART_H
