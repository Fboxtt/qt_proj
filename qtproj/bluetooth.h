//#ifndef BLUETOOTHMANAGER_H
//#define BLUETOOTHMANAGER_H

//#include <QObject>
//#include <QList>
//#include <QPair>
//#include <QString>
//#include <QDebug>

//// WinRT 头文件
//#include <winrt/Windows.Foundation.h>
//#include <winrt/Windows.Devices.Bluetooth.h>
//#include <winrt/Windows.Devices.Enumeration.h>
//#include <winrt/Windows.Devices.Bluetooth.Rfcomm.h>
//#include <winrt/Windows.Storage.Streams.h>

//using namespace winrt;
//using namespace Windows::Devices::Bluetooth;
//using namespace Windows::Devices::Enumeration;
//using namespace Windows::Devices::Bluetooth::Rfcomm;
//using namespace Windows::Storage::Streams;
//using namespace Windows::Foundation;

//class BluetoothManager : public QObject
//{
//    Q_OBJECT

//public:
//    explicit BluetoothManager(QObject *parent = nullptr);
//    void scanDevices();

//private:
//    void connectToDevice(const QString &deviceId);
//    void openRfcommService(const BluetoothDevice &device);
//    void sendData(const RfcommDeviceService &service);

//    QList<QPair<QString, QString>> deviceList; // 存储设备列表
//};

//#endif // BLUETOOTHMANAGER_H
