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
    QValueAxis *axisX;


    QMap<QString, line*> lineMap;
    void addNewLine(QString lineName, QString axisYName);
    void addNewPoint(QString lineName, int y);
};

#endif // CHART_H
