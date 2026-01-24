/*
 * adc.c
 *
 *  Created on: Jan 12, 2026
 *      Author: Aya Ramadan
 */

#include "adc.h"

#if ADC_ENABLED == STD_ON

static const char *g_TAG = TAG_ADC;

AdcDriver_t myAdc;
static bool adc_initialized = false;  


void Adc_Init(AdcDriver_t *driver)
{
    if (adc_initialized)  return ;
  
    //  Create ADC One-shot Unit
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT
    };
    adc_oneshot_new_unit(&unit_cfg, &driver->adc_handle);

    //  Configure channels ا
    for (adc_channel_t ch = ADC_CHANNEL_0; ch <= ADC_CHANNEL_7; ch++) {
        adc_oneshot_chan_cfg_t chan_cfg = {
            .bitwidth = ADC_RESOLUTION,
            .atten = ADC_ATTEN
        };
        adc_oneshot_config_channel(driver->adc_handle, ch, &chan_cfg);
    }

    //  Create Calibration
    adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id = ADC_UNIT,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_RESOLUTION
    };
   adc_cali_create_scheme_line_fitting(&cali_cfg, &driver->cali_handle);

   adc_initialized = true;
   #if ADC_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_TAG, "ADC Driver Initialized");
   #endif
  
}

void Adc_ReadRaw(AdcDriver_t *driver, adc_channel_t channel, int *raw_value)
{
    adc_oneshot_read(driver->adc_handle, channel, raw_value);
   #if ADC_DEBUG_ENABLED == STD_ON    
     ESP_LOGI(g_TAG, "ADC Raw[%d] = %d", channel, *raw_value);
   #endif 
}
 

#endif