import asyncio, logging, sys, platform, time

# —— 1. SelectorEventLoop 避免 Proactor/WinRT 冲突 —— 
if platform.system() == "Windows":
    asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

# —— 2. 强制 COM MTA —— 
sys.coinit_flags = 0

# —— 3. 引入 bleak 并允许 STA 回调 —— 
from bleak import BleakScanner, BleakClient
from bleak.backends.winrt.util import allow_sta
allow_sta()

# —— 4. DEBUG 日志 —— 
logging.basicConfig(level=logging.DEBUG)

TARGET_NAME = "ESP32_BLE"

async def minimal():
    print("→ 扫描设备…")
    devs = await BleakScanner.discover(timeout=3.0)
    for d in devs:
        if d.name == TARGET_NAME:
            addr = d.address
            print("→ 找到地址:", addr)
            break
    else:
        print("⚠️ 没扫描到目标设备，退出")
        return

    client = BleakClient(addr)
    try:
        # 加 5 秒超时
        await asyncio.wait_for(client.connect(), timeout=5.0)
        print("✅ Python: connect() 成功")
    except asyncio.TimeoutError:
        print("❌ connect() 超时")
        return
    except Exception as e:
        print("❌ connect() 抛出异常:", e)
        return

    # 测试断开
    await client.disconnect()
    print("🔌 Python: disconnect() 完成")

if __name__ == "__main__":
    while True:
        asyncio.run(minimal())
        time.sleep(5)
