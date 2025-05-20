#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ESP32Servo.h> // 用ESP32专用库

#define SERVO_PIN1 32
#define SERVO_PIN2 27
// 舵机的pin

#define CENTER_ANGLE 90
#define DELTA_ANGLE 40

Servo servo1;
Servo servo2;

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"           // UART服务UUID
#define CHARACTERISTIC_RX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // RX: 写入
#define CHARACTERISTIC_TX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" // TX: 通知

BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
int nowState = 1;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("BLE设备已连接");
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("BLE设备断开连接");
        pServer->startAdvertising(); // 断开后继续广播
    }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        String rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0)
        {
            Serial.print("收到数据：");
            for (int i = 0; i < rxValue.length(); i++)
                Serial.print(rxValue[i]);
            Serial.println();

            // 响应消息（示例：回传收到内容）
            String response = "你发了: " + rxValue;
            pTxCharacteristic->setValue(response);
            pTxCharacteristic->notify(); // 主动通知客户端

            if (rxValue == "SWITCH")
            {
                if (nowState == 1)
                {
                    nowState = -nowState;
                    servo1.write(CENTER_ANGLE + DELTA_ANGLE); // 150°
                    delay(1000);
                    servo2.write(CENTER_ANGLE - DELTA_ANGLE);
                    Serial.println("Received Y, rotate +60");
                }
                else if (nowState == -1)
                {
                    nowState = -nowState;
                    servo1.write(CENTER_ANGLE - DELTA_ANGLE); // 30°
                    delay(1000);
                    servo2.write(CENTER_ANGLE + DELTA_ANGLE);
                    Serial.println("Received N, rotate -60");
                }
            }
            else
            {
                // 尝试将收到的数据转为int
                int angle = rxValue.toInt();
                // toInt() 如果不是数字会返回0，所以你可以加个范围判断
                if (angle >= 0 && angle <= 90)
                {
                    servo1.write(CENTER_ANGLE-angle);
                    delay(1000);
                    servo2.write(CENTER_ANGLE+angle);
                    Serial.print("收到角度指令，转到：");
                    Serial.println(angle);
                }
                else
                {
                    Serial.println("收到无效指令");
                }
            }
        }
    }
};

void setup()
{
    servo1.attach(SERVO_PIN1);
    servo2.attach(SERVO_PIN2);

    servo1.write(CENTER_ANGLE);
    servo2.write(CENTER_ANGLE);

    Serial.begin(115200);
    Serial.println("初始化BLE...");

    BLEDevice::init("ESP32_BLE");

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    // 创建TX特征（通知用）
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_TX_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());

    // 创建RX特征（写入用）
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_RX_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();
    pServer->getAdvertising()->start();

    Serial.println("BLE已启动，等待连接...");
}

void loop()
{

    delay(1000);
}
