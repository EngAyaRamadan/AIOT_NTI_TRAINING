/*
 * gpio.h
 *
 *  Created on: Jan 8, 2026
 *      Author:Aya Ramadan
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "../../Cfg.h"


#if GPIO_ENABLED == STD_ON


#include "driver/gpio.h"
#include <stdint.h>
#include "esp_log.h"

/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif

/*------------ pointer for call back function in Interrupt mode------------------*/
typedef void (*Gpio_IsrCallback)(void *arg);

/*------------ GPIO Configuration Structure ------------------*/
typedef struct {
    gpio_num_t pin_num;         // GPIO pin number (e.g., GPIO_NUM_4)
    gpio_mode_t pin_mode;       // Pin mode: GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_OUTPUT_OD, etc.
    uint8_t pin_value;          // Initial output value (0 or 1), used only in output mode
    gpio_pull_mode_t pin_pull;   // Pull mode: GPIO_PULLUP_ONLY, GPIO_PULLDOWN_ONLY, GPIO_FLOATING, GPIO_PULLUP_PULLDOWN
} Gpio_ConfigType;

/*------------ Function prototypes ------------------*/
void Gpio_InitPin(Gpio_ConfigType *pinConfig); // Initialize a GPIO pin with full configuration
void Gpio_WritePinValue(gpio_num_t pin_num, uint8_t pin_value); // Write value to a GPIO pin (output mode)
void Gpio_TogglePinValue(gpio_num_t pin_num); // Read current value from a GPIO pin
uint8_t Gpio_ReadPinValue(gpio_num_t pin_num); // Read current value from a GPIO pin
// Interrupt APIs
void Gpio_EnableInterrupt(gpio_num_t pin_num,gpio_int_type_t int_type,Gpio_IsrCallback callback,void *arg);
void Gpio_DisableInterrupt(gpio_num_t pin_num);


#ifdef __cplusplus
}
#endif

#endif // GPIO_ENABLE

#endif /* GPIO_H_ */
