/*
 * ultrsonic.c
 *
 *  Created on: Jan 13, 2026
 *      Author: Aya Ramadan
 */
 
#include "ultrasonic.h" // Include Ultrasonic header

#if ULTRASONIC_ENABLED == STD_ON // Check if Ultrasonic feature is enabled

#include "esp_timer.h"
#include "rom/ets_sys.h"
#include "esp_attr.h"

static const char *g_TAG = TAG_ULTRASONIC ; // Debug TAG

/* Static variables to hold GPIO configuration for TRIG and ECHO pins */
static Gpio_ConfigType g_Trig_Pin;  
static Gpio_ConfigType g_Echo_Pin;  


/* ==================== Ultrasonic Initialization ==================== */
void Ultrasonic_Init(Ultrasonic_Config_t *Ultrasonic_Config)
{
    /* Configure TRIG pin as output */
    g_Trig_Pin.pin_num   = Ultrasonic_Config->trig_pin;  // Assign TRIG GPIO number
    g_Trig_Pin.pin_mode  = GPIO_MODE_OUTPUT;             // Set pin as output
    g_Trig_Pin.pin_value = 0;                            // Initialize output value to LOW
    
    Gpio_InitPin(&g_Trig_Pin);                // Initialize TRIG pin via GPIO driver
   
   // --------------------Debug information----------------------------//
    #if ULTRASONIC_DEBUG_ENABLED == STD_ON 
    ESP_LOGI(g_TAG, "Trigger pin Initialized"); // Log Trigger pin initialization
    #endif
   
   
    /* Configure ECHO pin as input */
    g_Echo_Pin.pin_num   = Ultrasonic_Config->echo_pin;  // Assign ECHO GPIO number
    g_Echo_Pin.pin_mode  = GPIO_MODE_INPUT;             // Set pin as input
   
    Gpio_InitPin(&g_Echo_Pin);              // Initialize ECHO pin via GPIO driver
     // --------------------Debug information----------------------------//   
    #if ULTRASONIC_DEBUG_ENABLED == STD_ON 
    ESP_LOGI(g_TAG, "ECHO pin Initialized"); // Log ECHO pin initialization
    #endif
    
    
    /* Ensure TRIG pin is LOW before starting measurement */
    Gpio_WritePinValue(g_Trig_Pin.pin_num,g_Trig_Pin.pin_value);  // Write LOW to TRIG pin
    
    // --------------------Debug information----------------------------//  
    #if ULTRASONIC_DEBUG_ENABLED == STD_ON 
    ESP_LOGI(g_TAG, "ULTRASONIC Initialized"); // Log ADC initialization
    #endif
    
 return;  // Return from initialization function
}


void Ultrasonic_Main(float *Distance)
{
	int64_t start_time = 0;
    int64_t end_time   = 0;
    int64_t timeout_start;
  
    /* ================= Trigger pulse ================= */
    g_Trig_Pin.pin_value = 0;
    Gpio_WritePinValue(g_Trig_Pin.pin_num, g_Trig_Pin.pin_value);
    ets_delay_us(2);

    g_Trig_Pin.pin_value = 1;
    Gpio_WritePinValue(g_Trig_Pin.pin_num, g_Trig_Pin.pin_value);
    ets_delay_us(10);

    g_Trig_Pin.pin_value = 0;
    Gpio_WritePinValue(g_Trig_Pin.pin_num, g_Trig_Pin.pin_value);

    /* ================= Wait for ECHO HIGH ================= */
    timeout_start = esp_timer_get_time();
    while (Gpio_ReadPinValue(g_Echo_Pin.pin_num) == 0)
    {
        if ((esp_timer_get_time() - timeout_start) > ULTRASONIC_TIMEOUT_US)
        {
            *Distance = -1.0f;   // Timeout error
            return;
        }
    }

    start_time = esp_timer_get_time();

    /* ================= Wait for ECHO LOW ================= */
    while (Gpio_ReadPinValue(g_Echo_Pin.pin_num) == 1)
    {
        if ((esp_timer_get_time() - start_time) > ULTRASONIC_TIMEOUT_US)
        {
            *Distance = -1.0f;   // Timeout error
            return;
        }
    }

    end_time = esp_timer_get_time();

    /*calculate Pulse width time*/
    int64_t pulse_width = end_time - start_time; // us
    /*calculate distance*/
    *Distance = (pulse_width * ULTRASONIC_SOUND_SPEED_CM_PER_US)
                / ULTRASONIC_DIV_FACTOR;
                
     #if ULTRASONIC_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_TAG, "Pulse: %lld us, Distance: %.2f cm",pulse_width, *Distance);
     #endif           
}

#endif 