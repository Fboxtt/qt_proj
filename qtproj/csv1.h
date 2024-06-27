#ifndef XLSX_H
#define XLSX_H
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
//#include "serial0.h"
#include <QTextBlock>
#include <Qmap>
#include <QTime>
#include <QVector>
#include "textDecode0.h"
class csv
{

public:
    csv();
    static void saveCsv(QString fileName, QStringList strListToCsv);
    static bool createDir(QString fileName);
    static void tbsToCsv(Ui::Widget *ui, QString fileName, textDcode* dcode0);
    static bool ReadCsv(QFile *file, QString fileName);
private:
    Ui::Widget *ui; //似乎是定义了一个类指针
//    QTimer *tim;
};

#endif // XLSX_H
