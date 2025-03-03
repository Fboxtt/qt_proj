#include "bluetooth.h"
#include <QDebug>
#include <QFile>

// 构造函数
BluetoothOTA::BluetoothOTA(QObject *parent) : QObject(parent) {
    // 初始化设备发现代理
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    qDebug() << QBluetoothDeviceDiscoveryAgent::supportedDiscoveryMethods();
    discoveryAgent->setLowEnergyDiscoveryTimeout(10000);
//    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
//            this, &BluetoothOTA::deviceDiscovered);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &BluetoothOTA::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothOTA::onScanFinished);
}

void BluetoothOTA::startDiscovered() {
    // 开始扫描设备
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod | QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BluetoothOTA::clear() {
    // 开始扫描设备
    devices.clear();
}

// 该方法可以扫描所有设备
void BluetoothOTA::onScanFinished()
{
    // 扫描完成后获取所有设备
    devices.clear();
    devices = discoveryAgent->discoveredDevices();

    qDebug() << "Scan finished. Total devices found:" << devices.size();

    // 打印所有设备信息
    for (const QBluetoothDeviceInfo &device : devices)
    {
        qDebug() << "Device:" << device.name() << "Address:" << device.address().toString();
    }
    devices.clear();
    // 退出应用程序
//    QCoreApplication::quit();
}

// 发现设备
void BluetoothOTA::deviceDiscovered(const QBluetoothDeviceInfo &device) {
    qDebug() << "Found device:" << device.name() << "Address:" << device.address().toString();
    if (device.name().contains("OTA Device")) {  // 替换为你的设备名称
        targetDevice = device;
        discoveryAgent->stop();
        connectToDevice();
    }
}

// 设备扫描错误
void BluetoothOTA::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error) {
    qWarning() << "Device scan error:" << error;
}

// 设备扫描完成
void BluetoothOTA::deviceScanFinished() {
    qDebug() << "Device scan finished.";
    if (targetDevice.isValid()) {
        qDebug() << "Target device found:" << targetDevice.name();
    } else {
        qWarning() << "Target device not found.";
    }
}

// 连接到设备
void BluetoothOTA::connectToDevice() {
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
void BluetoothOTA::deviceConnected() {
    qDebug() << "Device connected.";
    controller->discoverServices();
}

// 设备连接错误
void BluetoothOTA::deviceConnectionError(QLowEnergyController::Error error) {
    qWarning() << "Device connection error:" << error;
}

// 设备断开连接
void BluetoothOTA::deviceDisconnected() {
    qDebug() << "Device disconnected.";
}

// 发现服务
void BluetoothOTA::serviceDiscovered(const QBluetoothUuid &serviceUuid) {
    qDebug() << "Service discovered:" << serviceUuid.toString();
    if (serviceUuid == QBluetoothUuid(QString(OTA_SERVICE_UUID))) {  // 替换为你的 OTA 服务 UUID
        otaService = controller->createServiceObject(serviceUuid, this);
        if (otaService) {
            connect(otaService, &QLowEnergyService::stateChanged,
                    this, &BluetoothOTA::serviceStateChanged);
            otaService->discoverDetails();
        }
    }
}

// 服务发现完成
void BluetoothOTA::serviceDiscoveryFinished() {
    qDebug() << "Service discovery finished.";
}

// 服务状态变化
void BluetoothOTA::serviceStateChanged(QLowEnergyService::ServiceState state) {
    if (state == QLowEnergyService::ServiceDiscovered) {
        qDebug() << "Service details discovered.";
        sendOTAData();
    }
}

// 发送 OTA 数据
void BluetoothOTA::sendOTAData() {
    qDebug() << "Sending OTA data...";
    QFile file("firmware.bin");  // 替换为你的固件文件路径
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open firmware file.";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // 替换为你的 OTA 特征 UUID
    QString uuidString(OTA_CHARACTERISTIC_UUID);
    const QBluetoothUuid otaCharacteristicUuid(uuidString);
    QLowEnergyCharacteristic characteristic = otaService->characteristic(otaCharacteristicUuid);
    if (!characteristic.isValid()) {
        qWarning() << "OTA characteristic not found.";
        return;
    }

    // 发送数据
    otaService->writeCharacteristic(characteristic, data);
    qDebug() << "OTA data sent.";
}
