#ifndef SOCKET_H
#define SOCKET_H
#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QListWidgetItem>
#include <QLabel>

#include <QTcpServer>
#include <QTcpSocket>

#include <QJsonDocument>
#include <QJsonObject>
#include <QProgressDialog>
#include "ui_widget.h"
#include "widget.h"
#include "ui_widget.h"
class testObject {
public:
    testObject();
//    ~testProcess();

    enum STATUS {
        build,
        avalible,
        testing,
        tested,
    };
    enum COM_STATUS {
        NO_START,
        SENDING,
        RECEIVING,
        FAILED,
    };
    QStringList testNameList = {
        "握手中断测试",
        "发送HEX中断测试",
        "发送校验不成功测试",
        "发送错误命令后是否可以烧录",
        "测试各种相关NACK是否能产生",
        "正确烧录并完成之后功能是否正常",
        "是否能识别识别出丢包的报文",
        "",
    };
    QStringList cmdErrList = {
        "",
        "",
        "",
        "",
        "",
        "",
    };
    uint8_t status = build;
    uint8_t comStatus = SENDING;
    QString log = "";
    uint32_t packetNum;
    uint8_t testId = 0;
    QTcpServer *server;
    QTcpSocket *TcpSocket;
    QJsonObject sJsonObj;
    QJsonObject rJsonObj;

    int step = 0;

//    void serverSend(QString str);
//    void serverReceive(QByteArray hex);

    void QByteToJson(QByteArray);
    QByteArray jsonToQByte();
    void add(QString key);

    QMap<QString, void*> handleMap;
    QStringList keyNameList;
    int mapSize;

    void* pFunc(QString key);
    QString testingKey;

    QString writeStr; // 用于发送给BMS的字符串
    QString reportLog; // 测试报告
    void testProcess(QString key);
    void shakeInterrruptTest();
    void sendHexInterTest();
    void sendErrCheckTest();
    void sendErrCmdTest();
    void testNack();
    void losePacket();
    void normalDownload();


    QProgressDialog *bar = nullptr;

    // 新增的窗口用于放置烧录按钮
    QWidget *testWidget = nullptr;
    QGridLayout *layout = nullptr;
    void clear();
};


#endif // SOCKET_H
