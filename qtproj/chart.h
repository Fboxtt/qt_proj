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
    QTime firstTim;
    QMap<QString, QLineSeries*> seriesMap;
    void addNewLine(QString lineName, QString axisYName);
//    void addNewPoint(QString lineName, QTime inputTim, int newY);
};

class chartV
{
public:
    chartV();

    QMap<QString, chart*> chartMap;
    void addNewChart(QWidget *parent, QString chartName, QString lineName, QString axisYName);
    void lineAddPoint(QString lineName, QTime inputTim, int newY);
    void lineClearPoint(QString lineName);
    void ClearAllSeries(void);
};

#endif // CHART_H
