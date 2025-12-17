#include <Arduino.h>
// Pins
#define BTN_PIN 4
#define LED_PIN 2

// Queue + Mutex + EventGroup handles
QueueHandle_t xButtonQueue;
SemaphoreHandle_t xStateMutex;
EventGroupHandle_t xLedEventGroup;

// Event bits (LED modes)
#define LED_ON     (1 << 0)
#define LED_BLINK  (1 << 1)
#define LED_OFF    (1 << 2)

uint8_t globalState = 0;   // Shared state (protected by mutex)
uint8_t command_array[3]={1,2,3};
uint8_t i=0;
void SensorTask(void *pv)
{
  while (1)
  {
    static uint8_t lastState = HIGH;
    uint8_t currentState = digitalRead(BTN_PIN);
   if (currentState == LOW && lastState == HIGH)
   {
      xQueueSend(xButtonQueue, &command_array[i], 0);
      if ( i== 2)
        i=0;
       else
       i++;
      Serial.println("Sensor task finish");
   }
   lastState = currentState;
   vTaskDelay(pdMS_TO_TICKS(20));
  }
}
void ProcessorTask(void *pv)
{
  uint8_t receivedValue = 0;
  while (1)
  {
    if (xQueueReceive(xButtonQueue, &receivedValue, portMAX_DELAY))
    {
      // Protect update with mutex
      if (xSemaphoreTake(xStateMutex, portMAX_DELAY))
      {
        globalState = receivedValue;   // Update shared state
        Serial.println("the received value in process task is : "+String(globalState));
        xSemaphoreGive(xStateMutex);
      }
      // clear old events
      xEventGroupClearBits(
        xLedEventGroup,
        LED_ON | LED_BLINK | LED_OFF
       );

  // set new event
  switch (receivedValue)
  {
    case 1:
      xEventGroupSetBits(xLedEventGroup, LED_BLINK);
      break;

    case 2:
      xEventGroupSetBits(xLedEventGroup, LED_ON);
      break;

    case 3:
      xEventGroupSetBits(xLedEventGroup, LED_OFF);
      break;
  }
    }
  }
}

void ControllerTask(void *pv)
{
  while (1)
  {
    EventBits_t bits = xEventGroupWaitBits(
        xLedEventGroup,
        LED_ON | LED_BLINK | LED_OFF,
        pdTRUE,     // Clear bits when received
        pdFALSE,    // Wait for ANY bit
        portMAX_DELAY);
        
    if (bits & LED_ON)
    {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Led high");
    }
    else if (bits & LED_BLINK)
    {
      while (1)
      {
        EventBits_t check = xEventGroupGetBits(xLedEventGroup);

        if (check != 0) 
           break;  // Exit if new command arrives

        digitalWrite(LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(300));
        digitalWrite(LED_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(300));
        Serial.println("Led blynk");
      }
    }
    else if (bits & LED_OFF)
    {
      digitalWrite(LED_PIN, LOW);
      Serial.println("Led off");
    }
  }
}
void setup()
{
  Serial.begin(115200);
   pinMode(BTN_PIN, INPUT_PULLUP);
   pinMode(LED_PIN, OUTPUT);
  xButtonQueue = xQueueCreate(5, sizeof(uint8_t));
  xStateMutex = xSemaphoreCreateMutex();
  xLedEventGroup = xEventGroupCreate();

  xTaskCreate(SensorTask, "Sensor", 2048, NULL, 1, NULL);
  xTaskCreate(ProcessorTask, "Processor", 2048, NULL, 2, NULL);
  xTaskCreate(ControllerTask, "Controller", 2048, NULL, 3, NULL);
}

void loop() {}
