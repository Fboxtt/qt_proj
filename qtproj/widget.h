#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

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
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_selectFileButton_clicked();

    void on_conectSerialButton_clicked();

    void on_searchBtn_clicked();

    void on_openBtn_clicked();


    void on_sendBox_clicked();

    void on_clearReceiveDataButton_clicked();

    void SerialPortReadyRead_slot();
private:
    Ui::Widget *ui; //似乎是定义了一个类指针
    QTimer *tim;

public slots:
    void onTimeOut();
    void ReceveHexDecode();
};
#endif // WIDGET_H
