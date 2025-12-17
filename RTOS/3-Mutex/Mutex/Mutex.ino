/*
  T1 -->door Bill(gest)
  T2-->open door (owner)
*/

#include <Arduino.h>

SemaphoreHandle_t xSemaphore;

TaskHandle_t task1 =NULL;
TaskHandle_t task2 =NULL;
TaskHandle_t task3 =NULL;

void Task_1( void * pvParameters)
{
  vTaskDelay( 5000/portTICK_PERIOD_MS );
  while(1)
  {
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
    {
      Serial.println("Holed Mutex in task 1 for a long time");
      Serial.println("Give Mutex from 1");
      xSemaphoreGive( xSemaphore );
      vTaskDelay( 1000/portTICK_PERIOD_MS );
    }
  }
}
void Task_2( void * pvParameters)
{
  while(1)
  {
    Serial.println("Task_2 Running");
    vTaskDelay( 300/portTICK_PERIOD_MS );
  }
}
void Task_3( void * pvParameters)
{
  while(1)
  {
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
    {Serial.println("Holed Mutex for a long time in task 3");
    vTaskDelay( 2000/portTICK_PERIOD_MS );
    Serial.println("Give Mutex 3");
    xSemaphoreGive( xSemaphore );
     vTaskDelay( 1000/portTICK_PERIOD_MS );
    }
   
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xSemaphore = xSemaphoreCreateMutex();
  xTaskCreate(Task_1,"Task_1",1024, NULL,3,&task1 ); 
  xTaskCreate(Task_2,"Task_2",1024, NULL,2,&task2 ); 
  xTaskCreate(Task_3,"Task_3",1024, NULL,1,&task3 );
}

void loop() {
  // put your main code here, to run repeatedly:

}
