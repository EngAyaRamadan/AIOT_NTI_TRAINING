#ifndef PWM_H
#define PWM_H

#include <stdint.h>


void PWM_Init(uint8_t PWM_pin);
// duty_percent: 0.0 → 100.0
void PWM_SetDuty(uint8_t PWM_channel, float duty_percent);

#endif
