#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

/* Pin modes:
 *    OUTPUT
 *    INPUT
 *    INPUT_PULLUP
 */

/* GPIO APIs */
void GPIO_PinInit(uint8_t pin_number, uint8_t pin_mode);
void write_pin_Low(uint8_t pin_number);
void write_pin_high(uint8_t pin_number);
int read_pin(uint8_t pin_number);
void Toggle(uint8_t pin_number);

#endif