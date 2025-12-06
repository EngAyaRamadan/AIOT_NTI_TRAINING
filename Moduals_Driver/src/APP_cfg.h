#ifndef APP_CFG_H
#define APP_CFG_H

// General Definitions
#define STD_ON 1
#define STD_OFF 0

// ---------------------Module Enable-----------------------------------------
//HAL Module definitions
#define GPIO_ENABLED STD_ON
#define SENSORH_ENABLED STD_ON
#define UART_ENABLED STD_ON
#define WIFI_ENABLED STD_ON
// APP Module definitions
#define LED_ENABLED STD_ON
#define BUTTON_ENABLED STD_ON
#define POT_ENABLED STD_ON
#define LM35_ENABLED STD_ON
#define PWM_ENABLED STD_ON
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

// ---------------------Pin configration-----------------------------------------
// Pin Configuration
#define LED_PIN 5
#define BUTTON_PIN 18
#define POT_PIN 34
#define POT1_PIN 33
#define LM35_PIN 32
#define PWM_PIN  2
//Ultrasonic
#define TR_PIN   7
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
// Button
#define BUTTON_PRESSED   1
#define BUTTON_NOT_PRESSED   0



#endif
