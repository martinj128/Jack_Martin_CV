#ifndef _AWS_MQTT_H_
#define _AWS_MQTT_H_

#include <MQTTClient.h>
#include <WiFiClientSecure.h>
#include "secret.h"
#include "ArduinoJson.h"

#define AWS_IOT_PUBLISH_TOPIC "esp32/sensor/tempandhum"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

extern MQTTClient client;

void messageHandler(String &, String &);
void connectAWS(void);
void publishMessage(void);

#endif