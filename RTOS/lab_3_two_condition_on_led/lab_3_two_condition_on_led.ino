#include "Arduino.h"

#define BUTTON1_PIN 4
#define BUTTON2_PIN 5
#define LED_PIN     2

EventGroupHandle_t xEventGroup;

#define BTN1_BIT   (1 << 0)
#define BTN2_BIT   (1 << 1)


void vButton1Task(void *pvParameters)
{
  uint8_t laststate =HIGH;
  uint8_t currentstate ;
  while (1)
    {
        currentstate =digitalRead(BUTTON1_PIN);
        if (currentstate == LOW && laststate== HIGH )
        {
            xEventGroupSetBits(xEventGroup, BTN1_BIT);
            Serial.println("button 1 set");
        }
        else
        {
            xEventGroupClearBits(xEventGroup, BTN1_BIT);
            Serial.println("button 1 clear");
        }
    
         laststate = currentstate;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vButton2Task(void *pvParameters)
{
  uint8_t laststate =HIGH;
  uint8_t currentstate ;
  while (1)
    {
      currentstate =digitalRead(BUTTON2_PIN);
        if (currentstate == LOW && laststate== HIGH )
        {
            xEventGroupSetBits(xEventGroup, BTN2_BIT);
            Serial.println("button 2 set");
        }
         else
        {
            xEventGroupClearBits(xEventGroup, BTN2_BIT);
            Serial.println("button 2 clear");
        }
        laststate = currentstate;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void vLedTask(void *pvParameters)
{
    while (1)
    {
         vTaskDelay(pdMS_TO_TICKS(5));
         EventBits_t uxBits = xEventGroupGetBits(xEventGroup);
//        // Step 2: Both bits became ON → wait 1ms
//        vTaskDelay(pdMS_TO_TICKS(1));
//
//        // Step 3: Check again if bits STILL ON after 1ms
//        uxBits = xEventGroupGetBits(xEventGroup);
        
        if ((uxBits & (BTN1_BIT | BTN2_BIT)) == (BTN1_BIT | BTN2_BIT))
        {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED ON");
        }
        else
        {
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED OFF");
        }
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    // Create Event Group
    xEventGroup = xEventGroupCreate();
    // Create tasks
    xTaskCreate(vButton1Task, "BTN1", 1024, NULL, 1, NULL);
    xTaskCreate(vButton2Task, "BTN2", 1024, NULL, 1, NULL);
    xTaskCreate(vLedTask,    "LED",  1024, NULL, 1, NULL);

}

void loop() 
{
}
