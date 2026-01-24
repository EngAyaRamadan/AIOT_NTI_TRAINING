#include "PWM.h"
#include "../../APP_cfg.h"
#include <Arduino.h>


#if PWM_DEUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#define DEBUG_PRINT(var) Serial.print(var)
#else
#define DEBUG_PRINTLN(var)
#define DEBUG_PRINT(var)
#endif


void PWM_Init(uint8_t PWM_channel)
{
#if PWM_ENABLED == STD_ON
    pinMode(PWM_channel, OUTPUT);
	DEBUG_PRINTLN("PWM channel "+String(PWM_channel) +" Init");
#endif
}

// duty_percent: 0.0 → 100.0
void PWM_SetDuty(uint8_t PWM_channel, float duty_percent)
{
#if PWM_ENABLED == STD_ON
    if (duty_percent < 0.0f)
    {
        duty_percent = 0.0f;
		DEBUG_PRINTLN("duty_percent less than 0");
    }
    if (duty_percent > 100.0f)
    {
        duty_percent = 100.0f;
		DEBUG_PRINTLN("duty_percent greater than 100");
    }

    uint8_t pwm_value = (uint8_t)((duty_percent / 100.0f) * 255.0f);
    analogWrite(PWM_channel, pwm_value);
	DEBUG_PRINTLN("duty_percent equal to : "+String(duty_percent));
#endif
}
