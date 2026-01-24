#include "MQTT.h"
#include "../../APP_cfg.h"
#if MQTT_ENABLED == STD_ON

#if MQTT_DEUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#else
#define DEBUG_PRINTLN(var)
#endif

// MQTT client setup
WiFiClientSecure espClient;
PubSubClient Client(espClient);

void wifiConnect(void)
{
    DEBUG_PRINTLN("Connecting to WiFi: ");
    WiFi.begin(MQTT_WIFI_SSID, MQTT_WIFI_PASSWORD);
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20)
    {
        DEBUG_PRINTLN(".");
        delay(500);
        retry++;
    }
    espClient.setInsecure();
    DEBUG_PRINTLN("\nWiFi connected!");
}

void MqttClient_Init(const char *broker, uint16_t port, const char *clientID)
{
    DEBUG_PRINTLN("\nMQTT start INit");
    Client.setServer(broker, port);
    Client.setCallback(MqttClient_mqttCallback);

    while (!Client.connected())
    {
        DEBUG_PRINTLN("Connecting to MQTT broker...");
        if (Client.connect(clientID, MQTT_USER_NAME, MQTT_PASSWORD))
        {
            DEBUG_PRINTLN("Connected!");
            /*here you can subscribe to any topics*/
            String subTopic = String(SUBSCRIBE_TOPIC_NODE_ID) + "/#"; // "/#" mean any thing will start with  " SUBSCRIBE_TOPIC_NODE_ID/ "
            Client.subscribe(subTopic.c_str());
            delay(500); // Wait for a 500ms to ensure subscription is established
            DEBUG_PRINTLN("Subscribed to topic: ");
            DEBUG_PRINTLN(subTopic);
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.print(Client.state());
            Serial.println(" retrying in 2s");
            delay(2000);
        }
    }
}

void MqttClient_publishJSON(const char *topic, const char *key, float value, bool retain)
{
    StaticJsonDocument<200> doc;
    char jsonBuffer[200];

    doc[key] = value;
    serializeJson(doc, jsonBuffer);

    Client.publish(topic, jsonBuffer, retain);
}
void MqttClient_mqttCallback(char *topic, byte *payload, unsigned int length)
{
    char msg[length + 1];
    memcpy(msg, payload, length);
    msg[length] = '\0'; // null terminate

    Serial.print("Message arrived on topic: ");
    Serial.println(topic);
    Serial.print("Payload: ");
    Serial.println(msg);

    //   // Example: parse payload for "led on" or "led off"
    if (strcmp(topic, SUBSCRIBE_TOPIC_NODE_ID) == 0)
    {
        if (strncmp(msg, "ON", 2) == 0)
        {
            Serial.println(">>>>>>>>>>>>>>>>> led ON <<<<<<<<<<<<<<<<<<");
            digitalWrite(LED_PIN, HIGH);
        }
        else if (strncmp(msg, "OFF", 3) == 0)
        {
            Serial.println(">>>>>>>>>>>>>>>>> led OFF <<<<<<<<<<<<<<<<<<");
            digitalWrite(LED_PIN, LOW);
        }
        else
        {
            Serial.print("Unknown command: >>> ");
            Serial.println(msg);
        }
    }
    else
    {
        Serial.print("Unknown topic ");
    }
}
void MqttClient_Disconnect(void)
{
    Client.disconnect();
    Serial.println("MQTT Disconnected. Now idle.");
}
void MqttClient_loop(void)
{
    Client.loop();
    Serial.println("MQTT loop");
}

#endif // MQTT_CLIENT_MODULE