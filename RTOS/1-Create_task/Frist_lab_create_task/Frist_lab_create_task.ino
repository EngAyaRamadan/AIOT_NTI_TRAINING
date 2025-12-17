/* Create two tasks*/
#include <Arduino.h>

TaskHandle_t task1 =NULL;
TaskHandle_t task2 =NULL;

void Task1( void * pvParameters)
{
  while(1)
  {
    Serial.println("Task 1 Running");
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}
void Task2( void * pvParameters)
{
  while(1)
  {
    Serial.println("Task 2 Running");
    vTaskDelay(2000/portTICK_PERIOD_MS );
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate(Task1,"Task1",1024, NULL,1,&task1 ); 
  xTaskCreate(Task2,"Task2",1024, NULL,3,&task2 ); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
