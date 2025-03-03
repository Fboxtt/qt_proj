#include <QCoreApplication>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QLowEnergyController>
#include <QtBluetooth/QLowEnergyService>
#include <QFile>
#include <QDebug>

class BluetoothOTA : public QObject {
    Q_OBJECT

public:
    BluetoothOTA(QObject *parent = nullptr) : QObject(parent) {
        // 初始化设备发现代理
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                this, &BluetoothOTA::deviceDiscovered);
        connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
                this, &BluetoothOTA::deviceScanError);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
                this, &BluetoothOTA::deviceScanFinished);

        // 开始扫描设备
        discoveryAgent->start();
    }

private slots:
    // 发现设备
    void deviceDiscovered(const QBluetoothDeviceInfo &device) {
        qDebug() << "Found device:" << device.name() << "Address:" << device.address().toString();
        if (device.name().contains("OTA Device")) {  // 替换为你的设备名称
            targetDevice = device;
            discoveryAgent->stop();
            connectToDevice();
        }
    }

    // 设备扫描错误
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error) {
        qWarning() << "Device scan error:" << error;
    }

    // 设备扫描完成
    void deviceScanFinished() {
        qDebug() << "Device scan finished.";
        if (targetDevice.isValid()) {
            qDebug() << "Target device found:" << targetDevice.name();
        } else {
            qWarning() << "Target device not found.";
        }
    }

    // 连接到设备
    void connectToDevice() {
        qDebug() << "Connecting to device...";
        controller = QLowEnergyController::createCentral(targetDevice, this);
        connect(controller, &QLowEnergyController::connected,
                this, &BluetoothOTA::deviceConnected);
        connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
                this, &BluetoothOTA::deviceConnectionError);
        connect(controller, &QLowEnergyController::disconnected,
                this, &BluetoothOTA::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered,
                this, &BluetoothOTA::serviceDiscovered);
        connect(controller, &QLowEnergyController::discoveryFinished,
                this, &BluetoothOTA::serviceDiscoveryFinished);

        controller->connectToDevice();
    }

    // 设备连接成功
    void deviceConnected() {
        qDebug() << "Device connected.";
        controller->discoverServices();
    }

    // 设备连接错误
    void deviceConnectionError(QLowEnergyController::Error error) {
        qWarning() << "Device connection error:" << error;
    }

    // 设备断开连接
    void deviceDisconnected() {
        qDebug() << "Device disconnected.";
    }

    // 发现服务
    void serviceDiscovered(const QBluetoothUuid &serviceUuid) {
        qDebug() << "Service discovered:" << serviceUuid.toString();
        if (serviceUuid == QBluetoothUuid(OTA_SERVICE_UUID)) {  // 替换为你的 OTA 服务 UUID
            otaService = controller->createServiceObject(serviceUuid, this);
            if (otaService) {
                connect(otaService, &QLowEnergyService::stateChanged,
                        this, &BluetoothOTA::serviceStateChanged);
                otaService->discoverDetails();
            }
        }
    }

    // 服务发现完成
    void serviceDiscoveryFinished() {
        qDebug() << "Service discovery finished.";
    }

    // 服务状态变化
    void serviceStateChanged(QLowEnergyService::ServiceState state) {
        if (state == QLowEnergyService::ServiceDiscovered) {
            qDebug() << "Service details discovered.";
            sendOTAData();
        }
    }

    // 发送 OTA 数据
    void sendOTAData() {
        qDebug() << "Sending OTA data...";
        QFile file("firmware.bin");  // 替换为你的固件文件路径
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open firmware file.";
            return;
        }

        QByteArray data = file.readAll();
        file.close();

        // 替换为你的 OTA 特征 UUID
        QBluetoothUuid otaCharacteristicUuid(OTA_CHARACTERISTIC_UUID);
        QLowEnergyCharacteristic characteristic = otaService->characteristic(otaCharacteristicUuid);
        if (!characteristic.isValid()) {
            qWarning() << "OTA characteristic not found.";
            return;
        }

        // 发送数据
        otaService->writeCharacteristic(characteristic, data);
        qDebug() << "OTA data sent.";
    }

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothDeviceInfo targetDevice;
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *otaService = nullptr;

    // 替换为你的 OTA 服务 UUID 和特征 UUID
    static constexpr const char *OTA_SERVICE_UUID = "0000XXXX-0000-1000-8000-00805F9B34FB";
    static constexpr const char *OTA_CHARACTERISTIC_UUID = "0000YYYY-0000-1000-8000-00805F9B34FB";
};

//int main(int argc, char *argv[]) {
//    QCoreApplication app(argc, argv);

//    BluetoothOTA ota;
//    return app.exec();
//}

//#include "main.moc"
