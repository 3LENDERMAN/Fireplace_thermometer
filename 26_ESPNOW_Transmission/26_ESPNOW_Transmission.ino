#include <ESP8266WiFi.h>
#include <espnow.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// This file processes information from temperature sensor DS18B20 
// and sends it to reciever (Used hardware: esp8266, DS18B20)

uint8_t receiverMAC[] = {0x48, 0x55, 0x19, 0xec, 0xbe, 0x9e};

// Callback for sent data
void onDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Odesílání status: ");
  Serial.println(sendStatus == 0 ? "Úspěšné" : "Neúspěšné");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  sensors.begin();
  
  if (esp_now_init() != 0) {   // Inicialization of ESP-NOW protocol
    Serial.println("Chyba při inicializaci ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(onDataSent);
  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0); // Get temperature
  
  uint8_t data[sizeof(float)];
  memcpy(data, &temperatureC, sizeof(temperatureC)); // Load temperature to memory
  esp_now_send(receiverMAC, data, sizeof(data)); // Sent data
  delay(10000);
}