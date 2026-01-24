/*
 * Cfg.h
 *
 *  Created on: Jan 8, 2026
 *      Author:Aya Ramadan
 */

#ifndef CFG_H_
#define CFG_H_

//==============================Standard definitions ========================================
#define STD_ON      1 // Enabled
#define STD_OFF     0 // Disabled

//====================================	TAGS ================================================
//------------------Tags for Hal logging------------//
#define TAG_GPIO "GPIO_DRIVER"
#define TAG_NVS  "NVS_DRIVER"
#define TAG_WIFI "WIFI_DRIVER"
#define TAG_ADC  "ADC_DRIVER"
#define TAG_UART "UART_DRIVER"
#define TAG_I2C  "I2C_DRIVER"

//------------------Tags for App logging------------//
#define TAG_POT         "POT_DRIVER"
#define TAG_ULTRASONIC  "ULTRASONIC_DRIVER"
#define TAG_HTTP_CLIENT "HTTP_CLIENT_DRIVER"
#define TAG_HTTP_SERVER "HTTP_SERVER_DRIVER"
#define TAG_MQTT        "MQTT_DRIVER"

//====================================	Enable or disable ===================================
//------------------Enable or disable Hal drivers------------//
#define GPIO_ENABLED            STD_ON
#define NVS_ENABLED             STD_ON
#define WIFI_ENABLED            STD_ON
#define ADC_ENABLED             STD_ON
#define UART_ENABLED            STD_ON
#define I2C_ENABLED             STD_ON

//------------------Enable or disable App drivers------------//
#define POT_ENABLED             STD_ON
#define ULTRASONIC_ENABLED      STD_ON
#define HTTP_CLIENT_ENABLED     STD_ON
#define HTTP_SERVER_ENABLED     STD_ON
#define MQTT_ENABLED            STD_ON

//=============================Enable or disable debug ======================================
//------------------Enable or disable debug logs for Hal drivers------------//
// GPIO Debugging
#if GPIO_ENABLED == STD_ON
#define GPIO_DEBUG_ENABLED      STD_OFF
#endif

//NVS Debugging
#if NVS_ENABLED == STD_ON
#define NVS_DEBUG_ENABLED       STD_ON
#endif

//WIFI Debugging
#if WIFI_ENABLED == STD_ON
#define WIFI_DEBUG_ENABLED      STD_ON
#endif

//ADC Debugging
#if ADC_ENABLED == STD_ON
#define ADC_DEBUG_ENABLED       STD_ON
#endif

//UART Debugging
#if UART_ENABLED == STD_ON
#define UART_DEBUG_ENABLED      STD_ON
#endif

//I2C Debugging
#if I2C_ENABLED == STD_ON
#define I2C_DEBUG_ENABLED      STD_ON
#endif

//------------------Enable or disable debug logs for App drivers------------//
//POT Debugging
#if POT_ENABLED == STD_ON
#define POT_DEBUG_ENABLED       STD_ON
#endif

//ULTRSONIC Debugging
#if ULTRASONIC_ENABLED == STD_ON
#define ULTRASONIC_DEBUG_ENABLED  STD_ON
#endif

//HTTP client Debugging
#if HTTP_CLIENT_ENABLED == STD_ON
#define HTTP_CLIENT_DEBUG_ENABLED  STD_ON
#endif

//HTTP server Debugging
#if HTTP_SERVER_ENABLED  == STD_ON
#define HTTP_SERVER_DEBUG_ENABLED  STD_ON
#endif

//HTTP server Debugging
#if MQTT_ENABLED  == STD_ON
#define MQTT_DEBUG_ENABLED  STD_ON
#endif


//======================================Config ===========================================

//------------- Hal config ------------------------------------------

//------------- ADC Config ---------------------------
#define ADC_UNIT       ADC_UNIT_1
#define ADC_RESOLUTION ADC_BITWIDTH_12
#define ADC_MIN_VALUE  0
#define ADC_MAX_VALUE  4095
#define ADC_ATTEN      ADC_ATTEN_DB_12


//------------- UART Config ---------------------------
#define UART_BUAD_RATE   115200 
/*default init */
#define UART_FRAME_LENGTH UART_DATA_8_BITS
#define UART_PARITY      UART_PARITY_DISABLE
#define UART_STOP_BIT    UART_STOP_BITS_1
#define UART_CTRL        UART_HW_FLOWCTRL_DISABLE


//------------- Wifi config --------------------------
#define WIFI_MAX_RETRY 5


/*--------Hard Cred-------------*/

#define WIFI_STA_SSID    "Aya Ramadan"
#define WIFI_STA_PASS    "ayaramadan22"

#define WIFI_AP_SSID     "ESP32_Config"
#define WIFI_AP_PASS     ""


/*--------NVS Cred-------------*/
#define WIFI_NVS_NAMESPACE   "wifi"

#define WIFI_NVS_STA_SSID    "sta_ssid"
#define WIFI_NVS_STA_PASS    "sta_pass"

#define WIFI_NVS_AP_SSID     "ap_ssid"
#define WIFI_NVS_AP_PASS     "ap_pass"


//------------- App config ------------------------------------------

//------------- Pot Config ---------------------------

#define POT1_CHANNEL       ADC_CHANNEL_4   // 32
#define POT1_MIN_VALUE     0
#define POT1_MAX_VALUE     100

//------------- Ultrasonic Config ---------------------------
#define TRIGGER_GPIO_PIN                    5
#define ECHO_GPIO_PIN                       18
#define ULTRASONIC_SOUND_SPEED_CM_PER_US   (0.034f)     // Speed of sound in cm/us
#define ULTRASONIC_DIV_FACTOR              (2.0f)   // Division factor for distance calculation
#define ULTRASONIC_TIMEOUT_US              (30000U)

//------------- Http client Config ---------------------------
/* for HTTPS */
#define HTTP_CLIENT_USE_SSL 0
/*config client*/
#define HTTP_CLIENT_URL               "http://httpbin.org/post"  /*"https://httpbin.org/post"*/   /*"http://httpbin.org/get?device=ESP32&mode=GET"*/
#define HTTP_CLIENT_METHOD            HTTPCLIENT_METHOD_POST
#define HTTP_CLIENT_PAYLOAD          ""   /* such {\"device\":\"ESP32\",\"temp\":27,\"humidity\":60} assign in app*/
#define HTTP_CLIENT_CONTENET_TYPE    "application/json"
#define HTTP_CLIENT_TIMEOUT_MS       15000  // for http 5000
#define HTTP_CLIENT_CERT_PEM         NULL  /*httpbin_root_ca -> certficate*/  // for http or bundel-> NULL
/* retry */
#define MAX_RETRY       3
#define RETRY_DELAY_MS  2000


//------------- Http Server Config ---------------------------
/* for HTTPS */
#define HTTP_SERVER_USE_SSL    1
/*config server*/
#define HTTP_SERVER_PORT      443 /*https-> 443 , http-> 80*/               
     
//------------- MQTT Config ---------------------------
#define MQTT_USE_TLS               1
#define MQTT_USE_AUTHANTICATION    0

#define MQTT_BROKER_URI         "mqtts://bb29bf34b7f94af884e955f5719820b3.s1.eu.hivemq.cloud:8883"         /*mosqutto "mqtt://192.168.43.33:1883"*/  /*"mqtts://broker.hivemq.com"*/
#define MQTT_CLIENT_ID         "aya_esp32"
#define MQTT_USERNAME          "Aya_Ramadn"
#define MQTT_PASSWORD          "Ayaramadan22"
#define MQTT_CERT_PEM           NULL
/* length to copy subscribe payload */
#define MQTT_MAX_PAYLOAD 64 

#endif /* CFG_H_ */
