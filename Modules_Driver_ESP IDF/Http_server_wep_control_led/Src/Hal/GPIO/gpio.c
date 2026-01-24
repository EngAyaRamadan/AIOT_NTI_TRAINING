/*
 * gpio.c
 *
 *  Created on: Jan 8, 2026
 *      Author:Aya Ramadan
 */

#include "gpio.h"

#if GPIO_ENABLED == STD_ON

static const char *g_Tag = TAG_GPIO;

/*to check ISR service if installed before or not */
static uint8_t g_GpioIsrServiceInstalled = 0;
/*-------------------------- init function----------------------------*/
void Gpio_InitPin(Gpio_ConfigType *pinConfig)
{
	// reset previous config
	gpio_reset_pin( pinConfig->pin_num);
	
	 // Set pin direction (input/output/open-drain)
    gpio_set_direction(pinConfig->pin_num, pinConfig->pin_mode);

    // Configure pull-up/pull-down mode
    gpio_set_pull_mode(pinConfig->pin_num, pinConfig->pin_pull);

    // If pin is configured as output, set initial value
    if (pinConfig->pin_mode == GPIO_MODE_OUTPUT ||
        pinConfig->pin_mode == GPIO_MODE_OUTPUT_OD) {
        gpio_set_level(pinConfig->pin_num, pinConfig->pin_value);
    }

#if GPIO_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_Tag, "GPIO %d initialized - Mode: %d, Initial Value: %d, Pull: %d",
             pinConfig->pin_num, pinConfig->pin_mode, pinConfig->pin_value, pinConfig->pin_pull);
#endif
}
/*-------------------------- write function----------------------------*/
void Gpio_WritePinValue(gpio_num_t pin_num , uint8_t pin_value)
{
    gpio_set_level(pin_num, pin_value);

#if GPIO_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_Tag, "GPIO %d write - Value: %d", pin_num,pin_value);
#endif
}
/*-------------------------- read function----------------------------*/
uint8_t Gpio_ReadPinValue(gpio_num_t pin_num)
{
	 uint8_t value = gpio_get_level(pin_num);

#if GPIO_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_Tag, "GPIO %d read - Value: %d", pin_num, value);
#endif

    return value;
}
/*-------------------------- toggle function----------------------------*/
void Gpio_TogglePinValue(gpio_num_t pin_num)
{
	 gpio_set_level(pin_num, !(gpio_get_level(pin_num)));

#if GPIO_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_Tag, "GPIO %d Toggle ", pin_num);
#endif
}

/*-------------------------- Enable interrupt function----------------------------*/
void Gpio_EnableInterrupt(gpio_num_t pin_num,gpio_int_type_t int_type,Gpio_IsrCallback callback,void *arg)
{
    // Install ISR service once
    if (g_GpioIsrServiceInstalled == 0)
    {
        gpio_install_isr_service(ESP_INTR_FLAG_IRAM); // default flags
        g_GpioIsrServiceInstalled = 1;
    }

    // Set interrupt type (rising, falling, etc.)
    gpio_set_intr_type(pin_num, int_type);

    // Remove any previous ISR handler
    gpio_isr_handler_remove(pin_num);

    // Add ISR handler
    gpio_isr_handler_add(pin_num, callback, arg);

    // Enable interrupt
    gpio_intr_enable(pin_num);


#if GPIO_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_Tag, "GPIO %d interrupt enabled (type=%d)", pin_num, int_type);
#endif
}
/*-------------------------- Disable interrupt function----------------------------*/
void Gpio_DisableInterrupt(gpio_num_t pin_num)
{
    // Disable interrupt
    gpio_intr_disable(pin_num);

    // Remove ISR handler
    gpio_isr_handler_remove(pin_num);


#if GPIO_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_Tag, "GPIO %d interrupt disabled", pin_num);
#endif
}


#endif