#ifndef APP_CFG_H
#define APP_CFG_H

// General Definitions
#define STD_ON 1
#define STD_OFF 0

// Module definitions
#define GPIO_ENABLED STD_ON
#define SENSORH_ENABLED STD_ON
#define POT_ENABLED STD_ON
#define LM35_ENABLED STD_ON
#define ULTRASONIC_ENABLED STD_ON
#define UART_ENABLED STD_ON
#define LED_ENABLED STD_ON
#define WIFI_ENABLED STD_ON
#define GSM_ENABLED STD_ON
// Debug Definitions
#define GPIO_DEBUG STD_ON
#define SENSORH_DEBUG STD_ON
#define POT_DEBUG STD_ON
#define LM35_DEUG STD_ON
#define UART_DEBUG STD_ON
#define LED_DEBUG STD_ON
#define WIFI_DEBUG STD_ON
#define UULTRASONIC_DEBUG STD_ON
#define GSM_DEBUG STD_ON
// UART Configuration
#define UART_BUAD_RATE 9600
#define UART_FRAMELENGTH SERIAL_8N1
#define UART_TXPIN 17
#define UART_RXPIN 16

// Pin Configuration
#define POT_PIN 34
#define POT_RESOLUTION 12

#define MIN_Humdity_PERCENTAGE 0
#define MAX_umdity_PERCENTAGE 100

#define MIN_TEMP_PERCENTAGE 15
#define MAX_TEMP_PERCENTAGE 30

#define MIN_POT_VALUE 0
#define MAX_POT_VALUE (pow(2, POT_RESOLUTION) - 1)

// LM35
#define LM35_ADC_PIN 33
#define LM35_VREF 3.3f
#define LM35_ADC_MAX 4095.0f

// UCS
#define US_TRIG_PIN 5
#define US_ECHO_PIN 18
#define SOUND_SPEED_CM_PER_US 0.0343f
#define US_ECHO_TIMEOUT_US 30000UL
// LED
#define LED_1_PIN 7
#define LED_2_PIN 8
#define LED_3_PIN 9

#define SSID ""
#define PASSWORD ""

/*NEW*/
#define MIN_Humdity_PERCENTAGE 0
#define MAX_umdity_PERCENTAGE 100

#define MIN_TEMP_PERCENTAGE 15
#define MAX_TEMP_PERCENTAGE 30

#define POT_PIN 34
#define POT_LM_PIN 33
#define POT_HUM_PIN 32

#define LOW_TEMP_THRESHOLD 15
#define AVRAGE_TEMP_THRESHOLD 20
#define HIGH_TEMP_THRESHOLD 25

#define LED_PIN 7


#endif
