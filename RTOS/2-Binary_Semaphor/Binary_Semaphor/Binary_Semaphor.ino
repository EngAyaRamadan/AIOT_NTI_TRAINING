/*
  T1 -->door Bill(gest)
  T2-->open door (owner)
*/

#include <Arduino.h>

SemaphoreHandle_t xSemaphore;

TaskHandle_t task1 =NULL;
TaskHandle_t task2 =NULL;

void Gest_Task( void * pvParameters)
{
  while(1)
  {
    if( xSemaphoreGive( xSemaphore ) == pdTRUE )
    {
     Serial.println("Gest_Task Running");
     vTaskDelay( 1000/portTICK_PERIOD_MS );
    }
  }
}
void Owner_Task( void * pvParameters)
{
  while(1)
  {
    if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
    {
      Serial.println("owner_Task Running");
    }
  }
}
void setup() {
  Serial.begin(115200);
  xSemaphore = xSemaphoreCreateBinary();
  if (xSemaphore == NULL) 
  {
    Serial.println("Semaphore creation failed");
    while(1);
  }
  xTaskCreate(Gest_Task,"door Bill",1024, NULL,1,&task1 ); 
  xTaskCreate(Owner_Task,"open door",1024, NULL,1,&task2 );

}
void loop() {
  // put your main code here, to run repeatedly:

}
