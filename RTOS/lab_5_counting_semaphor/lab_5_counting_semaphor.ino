/*
 * one function multiple tasks
  T1 -->1
  T2-->2
  T3 -->3
  T4-->4
  T5 -->5
*/

#include <Arduino.h>

#define printer_number 3
SemaphoreHandle_t xSemaphore;

TaskHandle_t task1 =NULL;
TaskHandle_t task2 =NULL;
TaskHandle_t task3 =NULL;
TaskHandle_t task4 =NULL;

void print_Task( void * pvParameters)
{
  while(1)
  {
     char* person_print = (char*)pvParameters ;
   
    if(   xSemaphoreTake( xSemaphore, ( TickType_t ) 0 )  == pdTRUE )
    {
      Serial.println("-------------------------------------------------------------------");
      Serial.println("the person work at this time is"+String(person_print));
      vTaskDelay( 4000/portTICK_PERIOD_MS );
      Serial.println("the person finish work at this time is "+ String(person_print));
      xSemaphoreGive( xSemaphore );
      //Serial.println("-------------------------------------------------------------------");
    }
    else
    {
      Serial.println("No Printer Available");
      
         //vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  vTaskDelay(500 / portTICK_PERIOD_MS); 
}
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xSemaphore = xSemaphoreCreateCounting( printer_number, 2);
  xTaskCreate(print_Task,"A_Task",1024, (char*)"A",3,&task1 );
  xTaskCreate(print_Task,"B_Task",1024, (char*)"B",2,&task2 ); 
  xTaskCreate(print_Task,"C_Task",1024, (char*)"C",1,&task3 ); 
  xTaskCreate(print_Task,"D_Task",1024, (char*)"D",1,&task4);  
}

void loop() {
  // put your main code here, to run repeatedly:

}
