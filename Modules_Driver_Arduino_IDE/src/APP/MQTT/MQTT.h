#ifndef MQTT_MODULE_H
#define MQTT_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

void wifiConnect(void);
void MqttClient_Init(const char *broker, uint16_t port, const char *clientID);
void MqttClient_publishJSON(const char *topic, const char *key, float value, bool retain);
void MqttClient_mqttCallback(char *topic, byte *payload, unsigned int length);
void MqttClient_Disconnect(void);
void MqttClient_loop(void);

#endif
