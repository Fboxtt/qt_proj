#include <csv1.h>
#include <QFile>
#include <QTextCodec>

extern tbsStruct *tbsStru0;
csv::csv(void)
{

}

bool csv::createDir(QString fileName)
{
    QDir dir(fileName);
    if(!dir.exists()) {
        qDebug() << "not exist dir";
//        if(dir.mkpath(dir.absolutePath()))
//        {
//           qDebug() << "creat dir success:" << dir.absolutePath();
//        }
//        else
//        {
//            qDebug() << "creat dir failed" << dir.absolutePath();
//            return false;
//        }
    }
    return true;
}

void csv::tbsToCsv(Ui::Widget *ui, QString fileName, textDcode* dcode0)
{
    QStringList timeAndDataList, dataList;
    QStringList strListToCsv;
    QString strToCsv;
    QListWidgetItem *item;
    qDebug() << "listwidgetcount" << ui->listWidget->count();
    for(int idx = 0, limit = ui->listWidget->count(); idx < limit; idx++) {

        if(idx == 0) { // 写入标题
            strToCsv += "前缀,";
            foreach(dataCell cell, tbsStru0->dataCellList) {
                strToCsv += cell.valName + BAT_SEP;
            }
            strToCsv += "\n";
            strListToCsv.append(strToCsv);
            continue;
        }
        item = ui->listWidget->item(idx);


         strListToCsv.append(item->text() + "\n");
    }
    qDebug() << "strListToCsv" << strListToCsv;
    csv::saveCsv(fileName, strListToCsv);
}


void csv::saveCsv(QString fileName, QStringList strListToCsv)
{
    qDebug() << "in saveCsv func";


    if(csv::createDir(fileName) == false) {
        return;
    }

    QFile file;
    file.setFileName(fileName);
    if(!file.open(QIODevice::Append))
    {
        qDebug()<<"文件打开失败";
        file.close();
        return;
    }

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");

    foreach(QString str, strListToCsv) {
//        str += "\n";
        QByteArray encodedLine = codec->fromUnicode(str);
        file.write(encodedLine);
    }
    file.close();
    return;
}

bool csv::ReadCsv(QFile *file, QString fileName)
{ 

    file->setFileName(fileName);
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug()<<"文件打开失败";
        file->close();
        return false;
    }
//    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    return true;
}

QList<QList<int>> csv::ReadCsvFile(QFile *file)
{   
    QList<QList<int>> twoDimensionalList;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"文件打开失败";
        file->close();
        return twoDimensionalList;
    }

    QTextStream in(file);

    while (!in.atEnd()) {
        QString line = in.readLine();        
        QStringList fields = line.split(",");
        QList<int> list;
        for (const QString& field : fields) 
        {        
            list.append(field.toFloat() * 10);
        }
        twoDimensionalList.append(list);
        // for (int value : list) {
        //    // qDebug() << value;
        // }
        
       // qDebug()<<"---------------------------------";
    }
    file->close();   
    return twoDimensionalList;

}



