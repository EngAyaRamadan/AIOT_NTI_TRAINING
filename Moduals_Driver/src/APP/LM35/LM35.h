#ifndef LM35_H
#define LM35_H
#include <stdio.h>

void LM35_Init(uint8_t LM35_pin);
float LM35_main(uint8_t LM35_pin);
float LM35_value_Getter(void);

#endif
