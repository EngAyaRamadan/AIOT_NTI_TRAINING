/*
 -- > lab_4
*/

#include "Arduino.h"
#define BUTTON_PIN   4

SemaphoreHandle_t xButtonSemaphore;   

void vButtonTask(void *pvParameters)
{
  // Debounce variables
  int lastState = HIGH;
  TickType_t lastDebounceTick = 0;
  const TickType_t debounceTicks = pdMS_TO_TICKS(50);
    while (1)
    {
        int reading = digitalRead(BUTTON_PIN);
        if (reading == LOW && lastState == HIGH)
        {
            if ((xTaskGetTickCount() - lastDebounceTick) > debounceTicks)
            {
                // Button pressed → Give counting semaphore
                xSemaphoreGive(xButtonSemaphore);
                lastDebounceTick = xTaskGetTickCount();
            }
        }
        lastState = reading;
        vTaskDelay(10 / portTICK_PERIOD_MS);  
    }
}

// ---------------------------------------------------------
// Task 2: Every 2 seconds count how many presses occurred
// ---------------------------------------------------------
void vCountTask(void *pvParameters)
{
    while (1)
    {
        int pressCount = 0;
        // Collect all presses during last 2 seconds
        TickType_t endTime = xTaskGetTickCount() + pdMS_TO_TICKS(2000);
        while (xTaskGetTickCount() < endTime)
        {
            // Try take semaphore non-blocking
            if (xSemaphoreTake(xButtonSemaphore, 0) == pdTRUE)
            {
                pressCount++;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        Serial.print("Button Presses in last 2 seconds: ");
        Serial.println(pressCount);
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    // Create Counting Semaphore (max count 10, initial 0)
    xButtonSemaphore = xSemaphoreCreateCounting(10, 0);

    // Create tasks
    xTaskCreate(vButtonTask, "ButtonTask", 1024, NULL, 2, NULL);
    xTaskCreate(vCountTask,  "CountTask",  1024, NULL, 1, NULL);

}

void loop()
{
}
