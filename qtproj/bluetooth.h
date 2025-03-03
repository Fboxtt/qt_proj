#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QLowEnergyController>
#include <QtBluetooth/QLowEnergyService>

class BluetoothOTA : public QObject {
    Q_OBJECT

public:
    explicit BluetoothOTA(QObject *parent = nullptr);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void deviceScanFinished();
    void connectToDevice();
    void deviceConnected();
    void deviceConnectionError(QLowEnergyController::Error error);
    void deviceDisconnected();
    void serviceDiscovered(const QBluetoothUuid &serviceUuid);
    void serviceDiscoveryFinished();
    void serviceStateChanged(QLowEnergyService::ServiceState state);
    void sendOTAData();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothDeviceInfo targetDevice;
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *otaService = nullptr;

    // 替换为你的 OTA 服务 UUID 和特征 UUID
    static constexpr const char *OTA_SERVICE_UUID = "0000XXXX-0000-1000-8000-00805F9B34FB";
    static constexpr const char *OTA_CHARACTERISTIC_UUID = "0000YYYY-0000-1000-8000-00805F9B34FB";
};

#endif // BLUETOOTH_H
