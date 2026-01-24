#include <Arduino.h>
#include "../../App_cfg.h"
#include "../../Hal/SensorH/SensorH.h"
#include "LM35.h"

#if LM35_DEUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#define DEBUG_PRINT(var) Serial.print(var)
#else
#define DEBUG_PRINTLN(var)
#endif

// Sensor object
static SensorH_t config;
static uint16_t LM_value;

void LM35_Init(uint8_t LM35_pin)
{
#if LM35_ENABLED == STD_ON
    config.channel = LM35_pin;
    config.resolution = SENSORH_RESOLUTION;
    SensorH_Init(&config);
	DEBUG_PRINTLN("LM35 INIT Finish");
#endif
}

float LM35_main(uint8_t LM35_pin)
{
#if LM35_ENABLED == STD_ON
    LM_value = SensorH_ReadValue(LM35_pin);
    DEBUG_PRINT("TEMP_value: ");
    DEBUG_PRINTLN(LM_value);
	DEBUG_PRINTLN("LM35 Finish Read value");
    return LM_value;
#endif
}

float LM35_value_Getter(void)
{
#if LM35_ENABLED == STD_ON
	DEBUG_PRINTLN("LM35 Return value");
    return LM_value;
	
#endif
}
