#include <Arduino.h>
#include "../../App_cfg.h"
#include "../../HAL/GPIO/gpio.h"
#include "LED.h"

#if LED_DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#else
#define DEBUG_PRINTLN(var)
#endif

void LED_init(uint8_t LED_Pin)
{
#if LED_ENABLED == STD_ON
    GPIO_PinInit(LED_Pin, OUTPUT);
    DEBUG_PRINTLN("Iint LED " + String(LED_Pin));
#endif
}
void LED_ON(uint8_t LED_Pin)
{
#if LED_ENABLED == STD_ON
    write_pin_high(LED_Pin);
    DEBUG_PRINTLN("LED HIGH");
#endif
}
void LED_OFF(uint8_t LED_Pin)
{
#if LED_ENABLED == STD_ON
    write_pin_Low(LED_Pin);
    DEBUG_PRINTLN("LED LOW");
#endif
}
void LED_Toggle(uint8_t LED_Pin)
{
#if LED_ENABLED == STD_ON
    Toggle(LED_Pin);
    DEBUG_PRINTLN("LED Toggle");
#endif
}