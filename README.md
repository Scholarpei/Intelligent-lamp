# SCUT-Intelligent_lamp-计组大作业

## 项目简介

一个简单的基于esp32的寝室响指开关灯装置

识别响指部分参考代码+视频演示:

[B站up主 The_KOG 响指关灯! 宿舍智能化改造(已开源)](https://www.bilibili.com/video/BV1UP411K7Xt/)

代码库: https://github.com/TheKOG/InfinityGauntlet

本项目仅修改`connect.py`单个文件以从原先适配的经典蓝牙更改为适配BLE

请注意,我们的项目适配的灯种类为下面图片这种,如果与你的种类不同,请修改代码中的角度部分,或者切换为更加适合的安装角度:

![](https://raw.githubusercontent.com/Scholarpei/PicGo_picture/main/library/20250521000210915.png)

安卓端BLE控制软件:

![](https://raw.githubusercontent.com/Scholarpei/PicGo_picture/main/library/20250521002448804.png)

## 项目特点

- 简单易搭建
  - esp32连接简单,材料花费成本低,只需要 esp32(带蓝牙模块) + 2个180度舵机 即可
  - 程序易使用,手机端BLE控制程序开箱即用
- 效果拔群
  - 随手一打即是开关灯,优雅不失风度

## 硬件要求


### 组件
- ESP32: ESP32-WROOM-32 或类似型号
- 舵机: SG90/MG90S 180度舵机
- 电源: 5V/2A 电源适配器

## 软件环境

### 开发环境
- Arduino IDE 2.0+
- Python 3.8+
- Android Studio (用于开发控制APP)


完整依赖列表请参考 `requirements.txt`

## 项目结构

```
SCUT-Intelligent_lamp-计组大作业/
├── README.md                 # 项目说明文档
├── requirements.txt          # Python依赖列表
├── connect.py               # 响指识别的蓝牙连接程序
├── Intelligent-lamp.ino     # ESP32控制程序
├── control-app/            # 安卓控制端应用
├── esp32/                  # ESP32相关配置文件
├── pth/                    # 模型文件
└── audios/                 # 音频样本
```

## 安装说明

1. 硬件安装
   - 将ESP32连接到电脑
   - 连接两个舵机到ESP32的指定引脚（PIN 32和PIN 27）
   - 确保电源供应稳定

2. 软件配置
   ```bash
   # 安装Python依赖
   pip install -r requirements.txt
   
   # 安装Arduino IDE
   # 添加ESP32开发板支持
   # 安装必要的Arduino库：
   # - ESP32Servo
   # - BLE Device
   ```

3. 程序烧录
   - 使用Arduino IDE打开`Intelligent-lamp.ino`
   - 选择正确的开发板和端口
   - 编译并上传程序

## 使用说明

1. 启动系统
   - 给ESP32上电
   - 运行`connect.py`启动响指识别程序
   - 打开手机APP并连接到设备

2. 控制方式
   - 响指控制：在识别范围内打响指即可控制开关
   - APP控制：通过手机APP进行远程控制
   - 角度调节：可通过APP设置舵机转动角度

## 注意事项

- 确保舵机安装角度正确，参考图片中的安装方式
- 响指识别需要适当的背景噪音环境
- 首次使用需要校准舵机位置
- 建议定期检查舵机状态和连接稳定性

## 贡献者

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/jo-xin">
        <img src="https://avatars.githubusercontent.com/jo-xin" width="100px;" alt="jo-xin"/>
        <br />
        <sub><b>jo-xin</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/HWeng302">
        <img src="https://avatars.githubusercontent.com/HWeng302" width="100px;" alt="HWeng302"/>
        <br />
        <sub><b>HWeng302</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/Scholarpei">
        <img src="https://avatars.githubusercontent.com/Scholarpei" width="100px;" alt="Scholarpei"/>
        <br />
        <sub><b>Scholarpei</b></sub>
      </a>
    </td>
  </tr>
</table>

## 许可证

本项目采用 MIT 许可证

## 联系方式

如有问题或建议，请通过以下方式联系：
- 提交 Issue
- 发送邮件至：[1852055776@qq.com] 