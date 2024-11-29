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

// This file is without the calibration method (when the sensor is measuring correct value)

float temperature = 0.0;

void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&temperature, incomingData, sizeof(temperature));
  // Set value of recieved temperature to memory
  Serial.print("Recieved: ");
  Serial.println(temperature);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  WiFi.mode(WIFI_STA);

  u8g2.begin();

  // config pins to LEDs in RGB diode
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
    Serial.println("Setup of ESP-NOW failed");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  String tempValue = String(temperature);
  u8g2.clearBuffer();          
  u8g2.setFont(u8g2_font_logisoso46_tr); 
  tempValue.remove(tempValue.length() - 1);
  u8g2.drawStr(2, 55, tempValue.c_str());
  // Draw degrees circle sign
  u8g2.drawCircle(118, 11, 5);
  u8g2.drawCircle(118, 11, 4);
  u8g2.drawCircle(118, 11, 3);
  u8g2.sendBuffer();          
  
  turn_led_off();
  Serial.println(String(temperature));
  if (temperature < 68) { // show red 
    digitalWrite(RGB_R, HIGH);
  } else if (temperature < 72) { // show yellow
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, HIGH);
  } else if (temperature < 77) { // show green
    digitalWrite(RGB_G, HIGH);
  } else if (temperature < 80) { // show light blue
    digitalWrite(RGB_B, HIGH);
    digitalWrite(RGB_G, HIGH);
  } else if (temperature > 80.01) { // show blue
    digitalWrite(RGB_B, HIGH);
  }
  delay(5000);
}

void turn_led_off() {
  digitalWrite(LED_PIN1, LOW); 
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
}
