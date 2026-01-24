/*
 * pot.c
 *
 *  Created on: Jan 12, 2026
 *      Author: Aya Ramadan
 */

#include "pot.h"

#if POT_ENABLED == STD_ON

static const char *g_Tag = TAG_POT;

extern AdcDriver_t myAdc;

void Pot_Init(void)
{
	Adc_Init(&myAdc);
  #if POT_DEBUG_ENABLED == STD_ON
    ESP_LOGI(g_Tag, "POT Initialized");
   #endif
}

void Pot_ReadRaw(adc_channel_t channel ,int* rawvalue)
{
	 Adc_ReadRaw(&myAdc, channel, rawvalue);
     #if POT_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "POT on %d Raw Value is : %d ", channel,*rawvalue);
     #endif
}


void Pot_ReadMap(adc_channel_t channel ,int* mapvalue,int minValue , int maxValue)
{
	int value;
     Adc_ReadRaw(&myAdc, channel, &value);
     
      if (value < ADC_MIN_VALUE) value = ADC_MIN_VALUE;
      if (value > ADC_MAX_VALUE) value = ADC_MAX_VALUE;

    *mapvalue= (value - ADC_MIN_VALUE) * (maxValue - minValue) /
               (ADC_MAX_VALUE - ADC_MIN_VALUE) + minValue;
           
           
     #if POT_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "POT on %d Map Value is : %d ", channel,*mapvalue);
     #endif
}

#endif