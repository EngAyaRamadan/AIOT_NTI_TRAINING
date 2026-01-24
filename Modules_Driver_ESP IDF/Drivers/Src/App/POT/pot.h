/*
 * pot.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Aya Ramadan
 */

#ifndef POT_H_
#define POT_H_

#include "../../Cfg.h"

#if POT_ENABLED == STD_ON

#include <stdint.h>
#include <stdlib.h>
#include "esp_log.h"

#include "../../Hal/ADC/adc.h"

/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif


void Pot_Init(void);

void Pot_ReadRaw(adc_channel_t channel ,int* rawvalue);

void Pot_ReadMap(adc_channel_t channel ,  int* mapvalue,int minValue , int maxValue);


#ifdef __cplusplus
}
#endif


#endif // POT_ENABLED

#endif /* POT_H_ */
