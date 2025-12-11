#ifndef APP_CFG_H
#define APP_CFG_H

// General Definitions
#define STD_ON 1
#define STD_OFF 0

// ---------------------Module Enable-----------------------------------------
// HAL Module definitions
#define GPIO_ENABLED STD_ON
#define SENSORH_ENABLED STD_ON
#define UART_ENABLED STD_OFF
#define WIFI_ENABLED STD_OFF

// APP Module definitions
#define LED_ENABLED STD_ON
#define BUTTON_ENABLED STD_OFF
#define POT_ENABLED STD_ON
#define LM35_ENABLED STD_OFF
#define PWM_ENABLED STD_OFF
#define MQTT_ENABLED STD_ON

// ---------------------Module Debug-----------------------------------------
// HAL Debug Definitions
#define GPIO_DEBUG STD_ON
#define SENSORH_DEBUG STD_ON
#define UART_DEBUG STD_ON
#define WIFI_DEBUG STD_ON

// APP Debug Definitions
#define LED_DEBUG STD_ON
#define BUTTTON_DEBUG STD_ON
#define POT_DEBUG STD_ON
#define LM35_DEUG STD_ON
#define PWM_DEUG STD_ON
#define MQTT_DEUG STD_ON

// ---------------------Pin configration-----------------------------------------
// Pin Configuration
#define LED_PIN 5
#define BUTTON_PIN 18
#define POT_PIN 34
#define POT1_PIN 33
#define LM35_PIN 32
#define PWM_PIN 2
// Ultrasonic
#define TR_PIN 7
#define ECHO_PIN 6

// ---------------------Module Configration-----------------------------------------
// HAL Configuration

// ADC
#define SENSORH_RESOLUTION 12
#define MIN_SENSORH_VALUE 0
#define MAX_SENSORH_VALUE (pow(2, SENSORH_RESOLUTION) - 1)

#define SENSORH_VREF 3.3f
#define SENSORH_MAX 4095.0f

#define MIN_MAP_PERCENTAGE 0
#define MAX_MAP_PERCENTAGE 100

#define SOUND_SPEED_US 0.0346f
#define US_ECHO_TIMEOUT_US 30000UL

// UART Configuration
#define UART_BUAD_RATE 115200
#define UART_FRAMELENGTH SERIAL_8N1
#define UART_TXPIN 17
#define UART_RXPIN 16

// wifi parameter
#define WIFI_SSID "Aya Ramadan"
#define WIFI_PASSWORD "ayaramadan22"
#define TIMEOUT_MS 1500
#define WIFI_RECONNECT_TIMEOUT 3

// APP Configuration
// Button--------------------------
#define BUTTON_PRESSED 1
#define BUTTON_NOT_PRESSED 0
//-----------------------------------------
// MQTT wifi
#define MQTT_WIFI_SSID "Aya Ramadan"
#define MQTT_WIFI_PASSWORD "ayaramadan22"

// MQTT Broker
#define MQTT_BROKER "bb29bf34b7f94af884e955f5719820b3.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_CLIENT_ID "Aya_Esp"

#define MQTT_USER_NAME "Aya_Ramadan"
#define MQTT_PASSWORD "Ayaramadan22"

#define SUBSCRIBE_TOPIC_NODE_ID "SubAyaEsp"

// Will Message
#define WILL_TOPIC "client/Aya_Esp/status"
#define WILL_PAYLOAD "DISCONNECTED"
#define WILL_QOS 1
#define WILL_RETAIN true

/*===================== MQTT Broker Credentials  with pass local=====================*/
// #define MQTT_SERVER_IP "192.168.8.40" // the ip of the device running the broker
// #define MQTT_PORT 1883
// #define MQTT_USER_NAME "AyaRamadan"
// #define MQTT_PASSWORD "aya"
//-----------------------------------------------------------------
#endif
