# 📡 ESP32 Navigation Display (Roadie)

This project utilizes an **ESP32** with a **TFT display** and **Bluetooth** communication to display navigation information including wrapped text and a monochrome bitmap icon (received as Base64 data). Ideal for minimalistic turn-by-turn instructions in compact embedded systems.

---

## 🔧 Hardware Requirements

- **ESP32** (with Bluetooth capability)
- **TFT display** (tested with 128x128 resolution and [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library)
- Power supply for ESP32 (e.g., USB or LiPo)

---

## 📦 Libraries Used

Make sure to install the following libraries in your Arduino IDE or PlatformIO:

- [BluetoothSerial](https://www.arduino.cc/reference/en/libraries/bluetoothserial/)
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [Base64](https://github.com/adamvr/arduino-base64) (or any suitable alternative)
- `Arduino.h` (included by default in the ESP32 core)

---

## 🧠 Features

- Connects via **Bluetooth (Serial)** under the name `ESP32_NAV`
- Displays **wrapped text** instructions on the screen
- Decodes **monochrome bitmap icons** from Base64 string
- Efficient use of screen space with simple UI layout

---

## 🖼️ Payload Format

The ESP32 expects a newline-terminated string via Bluetooth in the following format:

TEXT:<instruction_text>|ICON:<base64_encoded_monochrome_bitmap> 
- `TEXT`: Message to display, automatically wrapped
- `ICON`: Base64 encoded 32x32 (monochrome, 1-bit) bitmap image  

## Android app

- [gitHub](https://github.com/pestana1213/Roadie)