#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>

#define LED_PIN1 D1
#define LED_PIN2 D2
#define RGB_R D7
#define RGB_G D6
#define RGB_B D5

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

float temperature = 0.0;

void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&temperature, incomingData, sizeof(temperature));
}

// Calibration data (table with measured and actual temperatures)
// NOTE: calibration data differs and are not needed if measurement is correct.
// For example this sensor was placed on hot pipes but not directly into fireplace (need for calibration)
struct CalibrationData {
  float realTemp;
  float measuredMin;
  float measuredMax;
};

CalibrationData calibrationTable[] = {
  {85, 60.7, 61.0},
  {84, 60.4, 60.7},
  {83, 60.2, 60.4},
  {82, 59.8, 60.2},
  {81, 59.4, 59.8},
  {80, 59.0, 59.4},
  {79, 58.6, 59.0},
  {78, 58.1, 58.6},
  {77, 57.6, 58.1},
  {76, 57.1, 57.6},
  {75, 56.7, 57.1},
  {74, 56.2, 56.7},
  {73, 55.7, 56.2},
  {72, 55.3, 55.7},
  {71, 54.9, 55.3},
  {70, 54.4, 54.9},
  {69, 54.1, 54.4},
  {68, 53.7, 54.1},
  {67, 53.1, 53.7},
  {66, 52.8, 53.1},
};

const int tableSize = sizeof(calibrationTable) / sizeof(calibrationTable[0]);

// Function that calibrates the difference between actual and measured value 
float calibrateTemperature(float measuredTemp) {
  // Find the corresponding value in calibration table to measured temperature
  for (int i = 0; i < tableSize - 1; i++) {
    if (measuredTemp >= calibrationTable[i].measuredMin && measuredTemp <= calibrationTable[i].measuredMax) {
      // Linear interpolation between two values
      float range = calibrationTable[i].measuredMax - calibrationTable[i].measuredMin;
      float ratio = (measuredTemp - calibrationTable[i].measuredMin) / range;
      float calibratedTemp = calibrationTable[i].realTemp * (1 - ratio) + calibrationTable[i + 1].realTemp * ratio;
      return calibratedTemp;
    }
  }
  
  if (measuredTemp > 25.1) {
    return measuredTemp + 16.8;
  }
  return measuredTemp + 1.9;
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  WiFi.mode(WIFI_STA);

  u8g2.begin(); // Start OLED display

  // Set pins for LEDs in RGB diode
  pinMode(LED_PIN1, OUTPUT);
  digitalWrite(LED_PIN1, LOW);
  pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN2, LOW);  
  pinMode(RGB_R, OUTPUT);
  digitalWrite(RGB_R, LOW);
  pinMode(RGB_G, OUTPUT);
  digitalWrite(RGB_G, LOW);
  pinMode(RGB_B, OUTPUT);
  digitalWrite(RGB_B, LOW);
  
  if (esp_now_init() != 0) {   // Initialization of ESP-NOW protocol
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  String tempValue = String(calibrateTemperature(temperature) + 1.9);
  float temp = calibrateTemperature(temperature) + 1.9;

  // Draw text of temperature value
  u8g2.clearBuffer();          
  u8g2.setFont(u8g2_font_logisoso46_tr); 
  tempValue.remove(tempValue.length() - 1);
  u8g2.drawStr(2, 55, tempValue.c_str());
  // Draw small upper degree circle
  u8g2.drawCircle(118, 11, 5);
  u8g2.drawCircle(118, 11, 4);
  u8g2.drawCircle(118, 11, 3);
  u8g2.sendBuffer();          
  turn_led_off();

  // For specific temperature light specific color:
  if (temp < 65.5) {
    turn_led_off();
  } else if (temp < 69.2) { // Below 69 emit red
    digitalWrite(RGB_R, HIGH);
  } else if (temp < 71.5) { // Below 71.5 emit yellow
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, HIGH);
  } else if (temp < 77.5) { // Below 77.5 emit green
    digitalWrite(RGB_G, HIGH);
  } else if (temp < 80.2) { // Below 80.2 emit light blue
    digitalWrite(RGB_B, HIGH);
    digitalWrite(RGB_G, HIGH);
  } else if (temp > 80.2) { // Else emit blue
    digitalWrite(RGB_B, HIGH);
  }
  delay(4000);
}

void turn_led_off() {
  digitalWrite(LED_PIN1, LOW); 
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
}
