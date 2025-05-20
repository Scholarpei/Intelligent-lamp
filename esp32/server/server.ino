#include <BLEDevice.h>
#include <BLEServer.h>
#include "Servo.h"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

Servo servo1,servo2;
static const int servoPin1 = 22;
static const int servoPin2 = 13;
static const int OpenParameter = 70;   //开启开关舵机的角度
static const int ResetParemeter = 0; //舵机复位角度
int state=0;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    // 创建新的客户端回调函数
  }

  void onDisconnect(BLEServer* pServer) {
    // 当客户端断开连接时调用
  }
  void onNotify(
    BLEServer* pSever,
    BLECharacteristic* pCharacteristic
  ) {
    // 获取字符串值
    std::string value = pCharacteristic->getValue();

    if (value == "SWITCH") {
      if(state==0){
        servo1.write(OpenParameter);
        delay(500);
        servo1.write(ResetParemeter);
      }else{
        servo2.write(OpenParameter-3);
        delay(500);
        servo2.write(ResetParemeter);
      }
      state=1-state;
    }
  }
};

// 用于保存连接的客户端
BLEServer *pServer;

void setup() {
  Serial.begin(115200);
  // 初始化蓝牙设备
  BLEDevice::init("ESP32_KOG");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // 创建蓝牙服务
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // 创建蓝牙特征
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->setValue("Hello World From ESP32");
  // 启动蓝牙服务
  pService->start();
  BLEDevice::startAdvertising();

  // 确保服务已成功创建
  if (pService == NULL) {
    Serial.println("Failed to create BLE service!");
    return;
  } else {
    Serial.println("BLE service created successfully!");
  }

  // 确保特征已成功创建
  if (pCharacteristic == NULL) {
    Serial.println("Failed to create BLE characteristic!");
    return;
  } else {
    Serial.println("BLE characteristic created successfully!");
  }

  delay(50);
  Serial.println("The device started, now you can pair it with bluetooth!");
  servo1.attach(
    servoPin1,
    Servo::CHANNEL_NOT_ATTACHED,
    0,
    180
  );
  servo1.write(ResetParemeter);
  servo2.attach(
    servoPin2,
    Servo::CHANNEL_NOT_ATTACHED,
    0,
    180
  );
  servo2.write(ResetParemeter);
}

void loop() {
  delay(1000);
}


