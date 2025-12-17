/*
  4- Events

*/
#include <Arduino.h>

#define sensor_bit    (1<<0)
#define communication_bit (1<<1)
#define user_interface_bit (1<<2)
#define storage_bit (1<<3)

EventGroupHandle_t xCreatedEventGroup;
EventBits_t uxBits;

TaskHandle_t task1 =NULL;
TaskHandle_t task2 =NULL;
TaskHandle_t task3 =NULL;
TaskHandle_t task4 =NULL;
TaskHandle_t task5 =NULL;

void Sensor_ready( void * pvParameters)
{
  while(1)
  {
   vTaskDelay( 200/portTICK_PERIOD_MS );
   xEventGroupSetBits(xCreatedEventGroup,sensor_bit );

   vTaskDelete(NULL);
  }
}
void commmunication_ready( void * pvParameters)
{
  while(1)
  {
   vTaskDelay( 100/portTICK_PERIOD_MS );
   xEventGroupSetBits(xCreatedEventGroup,communication_bit );

   vTaskDelete(NULL);
  }
}
void User_interface_ready( void * pvParameters)
{
  while(1)
  {
   vTaskDelay( 200/portTICK_PERIOD_MS );
   xEventGroupSetBits(xCreatedEventGroup,user_interface_bit );

   vTaskDelete(NULL);
  }
}
void Storage_ready( void * pvParameters)
{
  while(1)
  {
   vTaskDelay( 500/portTICK_PERIOD_MS );
   xEventGroupSetBits(xCreatedEventGroup,  storage_bit );
   vTaskDelete(NULL);
  }
}

void Boot_manager( void * pvParameters)
{
  while(1)
  { 
    EventBits_t uxBits;
    uxBits=xEventGroupWaitBits(xCreatedEventGroup,  sensor_bit | communication_bit,pdFALSE,pdFALSE,(TickType_t)1000 / portTICK_PERIOD_MS);
    if((uxBits & ( sensor_bit | communication_bit )  == ( sensor_bit | communication_bit)) )
    {
        Serial.println("Booting Phase 1");
      uxBits=xEventGroupWaitBits(xCreatedEventGroup,  sensor_bit | communication_bit |user_interface_bit | storage_bit,pdTRUE,pdTRUE,(TickType_t)6000 / portTICK_PERIOD_MS);
      if((uxBits & ( sensor_bit | communication_bit |user_interface_bit |storage_bit ) == ( sensor_bit | communication_bit |user_interface_bit |storage_bit )))
       {
         Serial.println("Booting start");
       }
    }

    vTaskDelay( 100/portTICK_PERIOD_MS );
   }
}
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xCreatedEventGroup = xEventGroupCreate();
  xTaskCreate(Sensor_ready,"Sensor_ready",1024, NULL,1,&task1 ); 
  xTaskCreate(commmunication_ready,"commmunication_ready",1024, NULL,1,&task2 ); 
  xTaskCreate(User_interface_ready,"User_interface_ready",1024, NULL,1,&task3 );
   xTaskCreate(Storage_ready,"Storage_ready",1024, NULL,1,&task4 ); 
  xTaskCreate(Boot_manager,"Boot_manager",1024, NULL,2,&task5 ); 
}

void loop() {
  // put your main code here, to run repeatedly:
}
