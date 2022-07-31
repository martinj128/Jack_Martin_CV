#include <Arduino.h>
#include "AWS_MQTT.h"
#include "ESPWiFi.h"
#include "DHT_Sensor.h"

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  setupWiFi();                        //Connect to home wifi
  connectAWS();                       //Connect to AWS IoT Core
  Serial.println("Starting Temp and Hum Sensor...");
  beginSensor();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  
  getSensorData();
  publishMessage();
  delay(5000);

}