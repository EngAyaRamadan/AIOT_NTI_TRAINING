/*
 * adc.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Aya Ramadan
 */

#ifndef ADC_H_
#define ADC_H_

#include "../../Cfg.h"

#if ADC_ENABLED == STD_ON

#include <stdint.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_log.h"

/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
    adc_oneshot_unit_handle_t adc_handle;
    adc_cali_handle_t cali_handle;
} AdcDriver_t;


void Adc_Init(AdcDriver_t *driver);

void Adc_ReadRaw(AdcDriver_t *driver, adc_channel_t channel, int *raw_value);




#ifdef __cplusplus
}
#endif


#endif // ADC_ENABLE

#endif /* ADC_H_ */
