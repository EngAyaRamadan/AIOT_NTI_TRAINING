#include <Arduino.h>
#include "../../App_cfg.h"
#include "../../Hal/SensorH/SensorH.h"
#include "POT.h"

#if POT_DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#define DEBUG_PRINT(var) Serial.print(var)
#else
#define DEBUG_PRINTLN(var)
#define DEBUG_PRINT(var)
#endif

// Sensor object
static SensorH_t config;
static float pot_value;

void POT_init(uint8_t POT_pin)
{
#if POT_ENABLED == STD_ON
    config.channel = POT_pin;
    config.resolution = SENSORH_RESOLUTION;
    SensorH_Init(&config);
	DEBUG_PRINTLN("POT INIT Finish");
#endif
}

float POT_main(uint8_t POT_pin)
{
#if POT_ENABLED == STD_ON
    pot_value = SensorH_ReadValue(POT_pin);
    DEBUG_PRINT("POT Value: ");
    DEBUG_PRINTLN(pot_value);
    DEBUG_PRINTLN("POT Finish Read value");
	return pot_value;
#endif
}

float POT_value_Getter(void)
{
#if POT_ENABLED == STD_ON
	DEBUG_PRINTLN("POT Return value");
    return pot_value;
#endif
}