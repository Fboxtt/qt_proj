#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QListWidgetItem>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget //继承是: 调用是::
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr); //构造函数，并且有默认参数
    ~Widget();

private slots:
    void on_selectFileButton_clicked();

    void on_conectSerialButton_clicked();

    void on_searchBtn_clicked();

    void on_openBtn_clicked();


    void on_sendBox_clicked();

    void on_clearReceiveDataButton_clicked();

    void SerialPortReadyRead_slot();

    void on_sendTbs_clicked();

    void SendAndDecode(QString sendData);

    void on_sendRegisterBox_clicked();

    void on_pushButton_3_clicked();

    void on_PopupRightMenu(const QPoint& pos);

    void on_clearReceiveDataButton_2_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui; //似乎是定义了一个类指针
    QTimer *tim;
    QTimer *tbsTim = nullptr;

public slots:
    void ReadSerialTimeOut();
    void SetTbsToTableAndChart(QListWidgetItem *item, int flag);
    void sendCmdRecieveWave();
};

#define BAT_SEP (",")
#define COMUT_SEP (";")
#define COMUT_BAT_SEP (";,")

#endif // WIDGET_H
