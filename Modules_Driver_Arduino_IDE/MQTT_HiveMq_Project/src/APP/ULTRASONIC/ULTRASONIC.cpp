#include <Arduino.h>
#include "../../App_cfg.h"
#include "../../HAL//GPIO/gpio.h"
#include "../../HAL/SensorH/SensorH.h"
#include "ULTRASONIC.h"

#if UULTRASONIC_DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)

#else
#define DEBUG_PRINTLN(var)

#endif

void Ultrasonic_Init(uint8_t TR_pin, uint8_t Echo_pin)
{
#if ULTRASONIC_ENABLED == STD_ON
    GPIO_PinInit(TR_pin, OUTPUT);
    GPIO_PinInit(Echo_pin, INPUT);
    write_pin_Low(TR_pin);
	DEBUG_PRINTLN("Ultrasonic Init");
#endif
}
static void Ultrasonic_TriggerPulse(uint8_t TR_pin)
{
#if ULTRASONIC_ENABLED == STD_ON
    write_pin_Low(TR_pin);
    delayMicroseconds(2);
    write_pin_high(TR_pin);
    delayMicroseconds(20);
    write_pin_Low(TR_pin);
	DEBUG_PRINTLN("Send Trigger");
#endif
}

float Ultrasonic_ReadDistanceCM(uint8_t TR_pin, uint8_t Echo_pin)
{
#if ULTRASONIC_ENABLED == STD_ON
    Ultrasonic_TriggerPulse(TR_pin);
    unsigned long duration = pulseIn(Echo_pin, HIGH, US_ECHO_TIMEOUT_US);
    if (duration == 0UL)
    {
        return -1.0f;
    }
	DEBUG_PRINTLN("duration equal to : "+ String(duration));
    return (duration * SOUND_SPEED_US) / 2.0f;
#endif
}
