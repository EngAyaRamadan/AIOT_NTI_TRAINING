/*
  4- Events

*/

#include <Arduino.h>


QueueHandle_t  mailbox;
TaskHandle_t task1 =NULL;
TaskHandle_t task2 =NULL;

typedef enum 
{START,STOP,INCRESE,DECRESE}CMD;


void Send_CMD( void * pvParameters)
{
  CMD sendValue = INCRESE;
  while(1)
  {
      Serial.println("Start to send");
      xQueueOverwrite( mailbox, &sendValue );
      Serial.println(" send Successfuly");
      vTaskDelay( 300/portTICK_PERIOD_MS );
  }
}

void Controller( void * pvParameters)
{
  CMD recivedValue;
  while(1)
  { 
        Serial.println("Start to receive");
        xQueueReceive(mailbox,&recivedValue,100/portTICK_PERIOD_MS );
      Serial.println(" received Successfuly");
      Serial.println(" the value received is: "+String(recivedValue));
  
      vTaskDelay( 200/portTICK_PERIOD_MS );
   }
}
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mailbox = xQueueCreate( 1, sizeof(CMD * ));
  xTaskCreate(Send_CMD,"Send_CMD",1024, NULL,1,&task1 ); 
  xTaskCreate(Controller,"Controller",1024, NULL,1,&task2 ); 

}

void loop() {
  // put your main code here, to run repeatedly:
}
