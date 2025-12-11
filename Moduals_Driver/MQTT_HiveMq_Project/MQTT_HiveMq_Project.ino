#include<Arduino.h>
#include"src/APP_cfg.h"
#include "src/APP/LED/LED.h"
#include "src/APP/POT/POT.h"
#include "src/APP/MQTT/MQTT.h"

const char* pubtopic = "Pot_sensor";
const char* subtopic = "SubAyaEsp";
void setup()
{
Serial.begin(115200
POT_init(POT_PIN);
wifiConnect();
MqttClient_Init(MQTT_BROKER, MQTT_PORT, MQTT_CLIENT_ID );
}
void loop()
{
float value = POT_main(POT_PIN);
value =map(value, MIN_SENSORH_VALUE, MAX_SENSORH_VALUE, MIN_MAP_PERCENTAGE, MAX_MAP_PERCENTAGE);
Serial.println("Map value for POT_PIN: " + String(value));

MqttClient_publishJSON(pubtopic, "Temp", value,false);
MqttClient_loop();
delay(500);
}
