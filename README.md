# Fireplace Thermometer

This project measures the temperature inside a fireplace but can be adapted for any application requiring temperature measurements in the range of **-30°C (-22°F) to 120°C (248°F)**.

## **System Overview**
The system consists of **two main components**:
1. **Transmitter**: Measures the temperature and sends the data wirelessly to the receiver using ESP-NOW.
2. **Receiver**: Receives the transmitted data, displays the temperature on an OLED screen, and activates an LED based on the reading.

### **Key Features**
- **Wireless Communication**: The transmitter and receiver communicate using the **ESP-NOW protocol**, ensuring power-efficient and reliable data transfer.
- **Range**: Tested transmission distances:
  - **Up to 50 meters** in clear, unobstructed environments.
  - **About 15 meters** indoors or with barriers (without additional antenna enhancements).
- **Low Power Consumption**:
  - Receiver: ~0.9W per hour
  - Transmitter: ~0.35W per hour
- **Temperature Units**: Displayed in degrees Celsius (°C).

---

## **Required Libraries**
To replicate or modify this project, install the following libraries:
- **[ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)**  
- **[espnow](https://github.com/saghonfly/SimpleEspNowConnection)**  
- **[OneWire](https://github.com/PaulStoffregen/OneWire)**  
- **[DallasTemperature](https://www.arduinolibraries.info/libraries/dallas-temperature)**  
- **[U8g2lib](https://github.com/olikraus/U8g2_Arduino)**  

---

## **Circuit Diagrams**

### **Receiver**
The receiver accepts temperature data from the transmitter over a wireless connection. It displays the value on an OLED screen and activates an LED based on the measured temperature.

![Receiver Circuit Diagram](https://github.com/user-attachments/assets/41ff5910-d584-4026-a1d4-a676741e9b86)

---

### **Transmitter**
The transmitter measures the temperature and sends data to the receiver using the ESP-NOW protocol. It operates on the physical and data-link layers for efficient communication. You will need the MAC address of the receiver's ESP8266 to pair the devices.

![Transmitter Circuit Diagram](https://github.com/user-attachments/assets/2601738d-8768-47ae-8c03-4928f0f1c257)

---

## **Final Build**
Below is the final appearance of the project (receiver side shown):

![Final Build](https://github.com/user-attachments/assets/0fb74159-aaec-4752-9e73-97507f11b79b)

---

## **Notes**
- Temperature readings are displayed in **degrees Celsius**.
- The system can be modified to measure other environmental parameters or adapt the display/output mechanism as needed.

---
