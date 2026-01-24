#include "Button.h"
#include "../../HAL/GPIO/gpio.h"
#include "../../APP_cfg.h"

#if BUTTTON_DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#else
#define DEBUG_PRINTLN(var)
#endif

// debouncing
#define SAMPLE_COUNT 1

static int sampleBuffer = 0;
static int lastStableState = HIGH;

void Button_Init(uint8_t button_pin)
{
#if BUTTON_ENABLED == STD_ON
    DEBUG_PRINTLN("Pin %d Initialized" + String(button_pin));
    GPIO_PinInit(button_pin, INPUT_PULLUP);
    lastStableState = digitalRead(button_pin);
    sampleBuffer = 0;
    DEBUG_PRINTLN("BUTTON Initialized Finsh");
#endif
}

bool Button_main(uint8_t button_pin)
{
#if BUTTON_ENABLED == STD_ON

    uint8_t readingValue = digitalRead(button_pin);
    DEBUG_PRINTLN("Read Button Value");
    Serial.println(readingValue);
    // Update Readings
    sampleBuffer = (sampleBuffer << 1) | (readingValue == LOW ? 1 : 0);
    sampleBuffer &= (1 << SAMPLE_COUNT) - 1;

    if (sampleBuffer == ((1 << SAMPLE_COUNT) - 1))
    {
        if (lastStableState != LOW)
        {
            lastStableState = LOW;
            DEBUG_PRINTLN("Pressed....");
            return BUTTON_PRESSED;
        }
    }
    else if (sampleBuffer == 0)
    {
        lastStableState = HIGH;
        DEBUG_PRINTLN("UnPressed....");
    }

    return BUTTON_NOT_PRESSED;

#endif
}
