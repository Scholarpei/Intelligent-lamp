import asyncio
import time
import sys
import logging
from bleak import BleakScanner, BleakClient
from bleak.backends.winrt.util import allow_sta

logging.basicConfig(level=logging.INFO)

allow_sta()  # 保证 WinRT 回调跑在 MTA

if sys.platform.startswith('win'):
    asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

TARGET_NAME = "ESP32_BLE"
CHAR_UUID   = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"  # RX: 写入

async def find_device():
    devices = await BleakScanner.discover(timeout=5.0)
    for d in devices:
        if d.name == TARGET_NAME:
            return d.address
    return None

async def connect_ble_with_retry(addr, max_retry=100):
    """带重试的BLE连接"""
    for i in range(max_retry):
        try:
            print(f"第{i+1}次尝试连接BLE...")
            client = BleakClient(addr, timeout=10.0)
            await client.connect()
            print("BLE连接成功！")
            return client
        except Exception as e:
            print(f"BLE连接失败：{e}")
            await asyncio.sleep(2)
    raise RuntimeError("多次尝试后仍无法连接BLE设备")

async def run_once():

    while True:
        addr = await find_device()
        print(">> 设备地址:", repr(addr))
        if not addr:
            print("找不到设备，请检查 TARGET_NAME，2秒后重试")
            await asyncio.sleep(2)
            continue
        print("Start Bleak with", addr)
        client = None
        try:
            client = await connect_ble_with_retry(addr)

            #必须延迟导入
            from record import Record
            import torchaudio
            from snap import is_Snap, Load
            Load('pth/best_pth', 'cpu')
            


            # 打印所有服务和特征
            for service in client.services:
                print(f"Service: {service.uuid}")
                for char in service.characteristics:
                    print(f"  Characteristic: {char.uuid}, properties: {char.properties}")
            while True:
                try:
                    Record("tmp/tmp.wav", debug=False)
                    waveform, _ = torchaudio.load("tmp/tmp.wav")
                    if waveform.numpy()[0].max() < 0.1:
                        print("音量过小，跳过")
                        continue
                    val = is_Snap("tmp/tmp.wav")
                    print("snap score:", val)
                    if val > 0.9:
                        await client.write_gatt_char(CHAR_UUID, b"SWITCH")
                        print(f"已发送 SWITCH")
                    else:
                        print(f"非响指")
                    await asyncio.sleep(1)
                except Exception as e:
                    print("循环内异常：", e)
        except Exception as e:
            print("BLE连接流程异常：", e)
        finally:
            if client and client.is_connected:
                await client.disconnect()
                print("BLE已断开，2秒后重连")
            await asyncio.sleep(2)

if __name__ == "__main__":

    while True:
        try:
            asyncio.run(run_once())
        except Exception as e:
            print("主循环异常：", e)
            time.sleep(5)
