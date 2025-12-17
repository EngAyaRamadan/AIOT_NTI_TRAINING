#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

/* ------------ RTOS Objects ------------ */
QueueHandle_t dataQueue;
SemaphoreHandle_t dangerSemaphore;
SemaphoreHandle_t serialMutex;
EventGroupHandle_t systemEventGroup;

/* ------------ Event Bits ------------ */
#define SYS_READY_BIT   (1 << 0)
#define DATA_OK_BIT     (1 << 1)

/* ------------ Data Structure ------------ */
typedef struct {
  int Speed;
  int distance;
  int temperature;
} SystemData_t;

/* ------------ Tasks ------------ */

void DataGeneratorTask(void *pvParameters)
{
  SystemData_t data;
  while (1)
  {
    data.Speed = random(0, 120);
    data.distance = random(5, 100);
    data.temperature = random(20, 60);

    xQueueSend(dataQueue, &data, portMAX_DELAY);

    xSemaphoreTake(serialMutex, portMAX_DELAY);
    Serial.printf("[GEN] Speed=%d Dist=%d Temp=%d\n",
                  data.Speed, data.distance, data.temperature);
    xSemaphoreGive(serialMutex);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void ProcessingTask(void *pvParameters)
{
  SystemData_t rxData;
  while (1)
  {
    xQueueReceive(dataQueue, &rxData, portMAX_DELAY);

    if (rxData.distance < 20)
    {
      xSemaphoreGive(dangerSemaphore);
    }
    else
    {
      xEventGroupSetBits(systemEventGroup, DATA_OK_BIT);
    }

    xSemaphoreTake(serialMutex, portMAX_DELAY);
    Serial.println("[PROC] Data processed");
    xSemaphoreGive(serialMutex);
  }
}

void ControlTask(void *pvParameters)
{
  while (1)
  {
    /* Danger Event */
    if (xSemaphoreTake(dangerSemaphore,portMAX_DELAY) == pdTRUE)
    {
      xSemaphoreTake(serialMutex, portMAX_DELAY);
      Serial.println("[CTRL] !!! EMERGENCY ACTION !!!");
      xSemaphoreGive(serialMutex);
    }

    /* Normal Operation */
    xEventGroupWaitBits(systemEventGroup,
                        SYS_READY_BIT | DATA_OK_BIT,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY);

    xSemaphoreTake(serialMutex, portMAX_DELAY);
    Serial.println("[CTRL] System running normally");
    xSemaphoreGive(serialMutex);
  }
}

void CommunicationTask(void *pvParameters)
{
  while (1)
  {
    xSemaphoreTake(serialMutex, portMAX_DELAY);
    Serial.println("[COMM] Status sent");
    xSemaphoreGive(serialMutex);

    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void LoggerTask(void *pvParameters)
{
  while (1)
  {
    xSemaphoreTake(serialMutex, portMAX_DELAY);
    Serial.println("[LOG] System alive");
    xSemaphoreGive(serialMutex);

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

/* ------------ Setup ------------ */
void setup()
{
  Serial.begin(115200);
  delay(1000);

  /* Create RTOS Objects */
  dataQueue = xQueueCreate(5, sizeof(SystemData_t));
  dangerSemaphore = xSemaphoreCreateBinary();
  serialMutex = xSemaphoreCreateMutex();
  systemEventGroup = xEventGroupCreate();

  xEventGroupSetBits(systemEventGroup, SYS_READY_BIT);

  /* Create Tasks */
  xTaskCreate(DataGeneratorTask, "DataGen", 2048, NULL, 1, NULL);
  xTaskCreate(ProcessingTask, "Process", 2048, NULL, 1, NULL);
  xTaskCreate(ControlTask, "Control", 2048, NULL, 1, NULL);
  xTaskCreate(CommunicationTask, "Comm", 2048, NULL, 1, NULL);
  xTaskCreate(LoggerTask, "Logger", 2048, NULL, 1, NULL);
}

void loop()
{
  /* Nothing here – RTOS is running */
}
