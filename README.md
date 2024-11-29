# Fireplace_thermometer
This project is designed to measure temperature in fireplace, but can be used to measure anything in interval from -30°C (-22°F) to 120°C (248°F) 

**There are two parts**: transmitter (measures temperatures and sends signal directly to another ESP) and **reciever** (recieves signal and displays it on OLED + turns specific LED on)
The distance between two ESPs in clear space can be up to 50 meters (tested) and about 15 meters inside house or with barriers.

## Used libraries:
**ESP8266WiFi**

**espnow**

**OneWire**

**DallasTemperature**

**U8g2lib**

## Circuit diagram
Reciever: recieves measured temperature from transmitter (wireless connection) attached display showing the measured value. 


![ESP_reciever](https://github.com/user-attachments/assets/41ff5910-d584-4026-a1d4-a676741e9b86)

Transmitter: transmits signal using ESP-NOW protocol (needed MAC adress of reciever ESP8266) on physical and data-link layer (data and power efficient)


![ESP_transmission](https://github.com/user-attachments/assets/2601738d-8768-47ae-8c03-4928f0f1c257)

NOTE: Measured temperature is in degrees Celcius.
