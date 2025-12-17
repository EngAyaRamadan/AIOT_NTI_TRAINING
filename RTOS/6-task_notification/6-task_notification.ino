#include <Arduino.h>
TaskHandle_t TaskA_Handle = NULL;

void TaskA(void *pvParameters)
{
  while (1)
  {
    Serial.println("Task A: Waiting for notification...");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Task A: Notification received!");
  }
}

void TaskB(void *pvParameters)
{
  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(3000));
    Serial.println("Task B: Sending notification");
    xTaskNotifyGive(TaskA_Handle);
  }
}

void setup()
{
  Serial.begin(115200);
 xTaskCreate(TaskA,"Task A",2048,NULL, 2,&TaskA_Handle);
 xTaskCreate(TaskB,"Task B",2048,NULL,1,NULL);
 // xTaskCreatePinnedToCore(TaskB,"Task B",2048,NULL,1,NULL,1);
}

void loop()
{
}
