#ifndef TEXTDECODE_H
#define TEXTDECODE_H

#endif // TEXTDECODE_H

#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
//#include "serial0.h"
#include <QTextBlock>
#include <Qmap>

class textDcode
{

public:
    textDcode();

    QString DecodeHexToCommand(Ui::Widget *ui, QString decodeStr);
    QString SendCmdDocode(QStringList hexList, QString decodeStr);
    QString ByteDecode(QMap<int, QString> mapCode, int keys);
    QMap<int,QString> typeCode;
    QMap<int,QString> funcCode;
    QMap<int,QString> ackCode;
private:

};
