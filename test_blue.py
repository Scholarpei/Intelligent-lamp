import sys
import asyncio

if sys.platform.startswith('win'):
    asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

from bleak import BleakClient
# from record import Record
# from snap import is_Snap
# import torchaudio

ADDR = "8C:4F:00:C8:3B:7E"
CHAR_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

async def main():
    print("Connecting...")
    client = BleakClient(ADDR, timeout=10.0)
    await client.connect()
    print("Connected!")

    # 延迟导入
    from record import Record
    import torchaudio
    from snap import is_Snap, Load


    Load('pth/best_pth', 'cpu')

    for i in range(10):
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
                print(f"已发送 SWITCH {i}")
            else:
                await client.write_gatt_char(CHAR_UUID, f"TEST{i}".encode())
                print(f"已发送 TEST{i}")
            await asyncio.sleep(1)
        except Exception as e:
            print("循环内异常：", e)
    await client.disconnect()

    # for i in range(10):
    #     try:
    #         print("准备录音")
    #         Record("tmp/tmp.wav", debug=False)
    #         print("录音完成")
    #         waveform, _ = torchaudio.load("tmp/tmp.wav")
    #         if waveform.numpy()[0].max() < 0.1:
    #             print("音量过小，跳过")
    #             continue
    #         val = is_Snap("tmp/tmp.wav")
    #         print("snap score:", val)
    #         if val > 0.9:
    #             await client.write_gatt_char(CHAR_UUID, b"SWITCH")
    #             print(f"已发送 SWITCH {i}")
    #         else:
    #             await client.write_gatt_char(CHAR_UUID, f"TEST{i}".encode())
    #             print(f"已发送 TEST{i}")
    #         await asyncio.sleep(1)
    #     except Exception as e:
    #         print("循环内异常：", e)
    # await client.disconnect()

asyncio.run(main())