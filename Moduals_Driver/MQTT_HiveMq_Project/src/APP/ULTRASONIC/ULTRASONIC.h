#ifndef ULTRASONIC_H
#define ULTRASONIC_H

void Ultrasonic_Init(uint8_t TR_pin, uint8_t Echo_pin);
float Ultrasonic_ReadDistanceCM(uint8_t TR_pin, uint8_t Echo_pin);

#endif
