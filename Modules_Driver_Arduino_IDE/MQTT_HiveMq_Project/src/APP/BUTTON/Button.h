#ifndef _BUTTON_H
#define _BUTTON_H

#include <Arduino.h>
#include <stdint.h>

void Button_Init(uint8_t button_pin);
bool Button_main(uint8_t button_pin);

#endif