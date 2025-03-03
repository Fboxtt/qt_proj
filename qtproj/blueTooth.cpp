#include "bluetooth.h"

BluetoothManager::BluetoothManager(QObject *parent) : QObject(parent)
{
    // 初始化 WinRT
    winrt::init_apartment();

    // 扫描设备
    scanDevices();
}

void BluetoothManager::scanDevices()
{
    qDebug() << "Scanning for Bluetooth devices...";

    // 获取蓝牙设备选择器
    auto deviceSelector = BluetoothDevice::GetDeviceSelector();
    DeviceInformation::FindAllAsync(deviceSelector).Completed([this](IAsyncOperation<DeviceInformationCollection> const &sender, AsyncStatus const status) {
        if (status == AsyncStatus::Completed) {
            auto devices = sender.GetResults();
            for (const auto &device : devices) {
                QString deviceName = QString::fromStdString(to_string(device.Name()));
                QString deviceId = QString::fromStdString(to_string(device.Id()));

                qDebug() << "Found device:" << deviceName << "ID:" << deviceId;
                deviceList.append({deviceName, deviceId});
            }

            // 模拟选择第一个设备
            if (!deviceList.isEmpty()) {
                qDebug() << "Selecting first device:" << deviceList[0].first;
                connectToDevice(deviceList[0].second);
            }
        } else {
            qDebug() << "Device scan failed.";
        }
    });
}

void BluetoothManager::connectToDevice(const QString &deviceId)
{
    qDebug() << "Connecting to device:" << deviceId;

    // 获取设备
    BluetoothDevice::FromIdAsync(to_hstring(deviceId)).Completed([this](IAsyncOperation<BluetoothDevice> const &sender, AsyncStatus const status) {
        if (status == AsyncStatus::Completed) {
            auto device = sender.GetResults();
            qDebug() << "Connected to device:" << QString::fromStdString(to_string(device.Name()));

            // 打开 RFCOMM 服务
            openRfcommService(device);
        } else {
            qDebug() << "Failed to connect to device.";
        }
    });
}

void BluetoothManager::openRfcommService(const BluetoothDevice &device)
{
    qDebug() << "Opening RFCOMM service...";

    // 获取 RFCOMM 服务
    RfcommDeviceService::FromIdAsync(device.DeviceId()).Completed([this](IAsyncOperation<RfcommDeviceService> const &sender, AsyncStatus const status) {
        if (status == AsyncStatus::Completed) {
            auto service = sender.GetResults();
            qDebug() << "RFCOMM service opened.";

            // 发送数据
            sendData(service);
        } else {
            qDebug() << "Failed to open RFCOMM service.";
        }
    });
}

void BluetoothManager::sendData(const RfcommDeviceService &service)
{
    qDebug() << "Sending data...";

    // 创建数据写入器
    DataWriter writer;
    writer.WriteString(to_hstring("Hello from Qt!"));

    // 发送数据
    service.Connection().OutputStream().WriteAsync(writer.DetachBuffer()).Completed([this](IAsyncOperation<uint32_t> const &sender, AsyncStatus const status) {
        if (status == AsyncStatus::Completed) {
            qDebug() << "Data sent successfully.";
        } else {
            qDebug() << "Failed to send data.";
        }

        // 退出应用程序
        QCoreApplication::quit();
    });
}
