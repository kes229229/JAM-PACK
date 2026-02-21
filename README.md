<div align="center">

# 📡 JAM-PACK

**Ultimate ESP32 Firmware · v1.0.0 · BETA**

[![Version](https://img.shields.io/badge/version-1.0.0-ff8c00?style=for-the-badge&logo=arduino&logoColor=white)](https://github.com)
[![Status](https://img.shields.io/badge/status-BETA-red?style=for-the-badge&logoColor=white)](https://github.com)
[![Platform](https://img.shields.io/badge/platform-ESP32-ff6600?style=for-the-badge&logo=espressif&logoColor=white)](https://github.com)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979d?style=for-the-badge&logo=arduino&logoColor=white)](https://github.com)
[![License](https://img.shields.io/badge/license-MIT-brightgreen?style=for-the-badge)](https://github.com)

</div>

---

## ⚡ О проекте

**JAM-PACK** — мощная универсальная прошивка для **ESP32** с дисплеем **SSD1306 128×64**.  
Объединяет инструменты для исследования WiFi, Bluetooth, SubGHz, Infrared и GPIO в одном устройстве.

> 🚧 **BETA версия** — прошивка находится в активной разработке. Возможны баги и недоработки.

> ⚠️ **Только для образовательных и исследовательских целей.** Используй в соответствии с законами своего региона.

---

## 🔧 Железо

| Компонент | Модель | Назначение |
|-----------|--------|------------|
| MCU | ESP32-WROOM | Основной контроллер |
| Дисплей | SSD1306 128×64 | Интерфейс |
| SubGHz | CC1101 | 300-928 MHz |
| 2.4GHz | NRF24L01 | 2.4 GHz |
| IR | TX + RX | Инфракрасный |
| Хранилище | SD Card | FAT32 |

---

## 📡 Функции

<table>
<tr>
<td>

**📶 WiFi**
- Deauther (выбор AP)
- Beacon Spam
- EvilPortal
- Wardriving

</td>
<td>

**🔵 Bluetooth**
- BLE Spam iOS
- BLE Spam Android
- BLE Spam Windows
- BadBLE

</td>
<td>

**📡 SubGHz**
- SubRead
- SubSend
- Analyzer
- Bruteforce

</td>
</tr>
<tr>
<td>

**🔴 Infrared**
- IR Send
- IR Read
- TV / PJ / AC OFF

</td>
<td>

**⚡ GPIO**
- iButton Read / Write
- NRF24 Spectrum
- NRF24 Config
- Serial [DEV]

</td>
<td>

**⚙ Settings**
- Display Color
- Standby Time
- About

</td>
</tr>
</table>

---

## 🔌 Схема подключения

```
📺 SSD1306 Display          🔘 Buttons
   VCC  → 3V3                  UP   → G27
   GND  → GND                  DOWN → G26
   SCL  → G22                  OK   → G33
   SDA  → G21                  BACK → G32

📡 CC1101                   💡 IR
   1    → GND                  IR-T → G12
   2    → 3V3                  IR-R → G14
   3    → G4
   4    → G15               🔌 GPIO
   5    → G18                  A    → G2
   6    → G23                  B    → G25
   7    → G19                  C    → G17
                               D    → G16
💾 SD Card                     E    → G13
   3V3  → 3V3                  F    → G12
   CS   → G5
   MOSI → G23
   CLK  → G18
   MISO → G19
   GND  → GND
```

---

<div align="center">

**JAM-PACK · Ultimate Edition · v1.0.0 · 2026**

*ESP32 · SSD1306 · CC1101 · NRF24 · IR · BLE*

</div>
