/*
 * button debouncing
*/
#include <Arduino.h>
#include "src/APP/LED.h"
#define LED 2
#define BUTTON 4

SemaphoreHandle_t xSemaphore;

TaskHandle_t task1 =NULL;
TaskHandle_t task2 =NULL;

void toggle_led( void * pvParameters)
{
  while(1)
  {  
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {
      LED_Toggle(LED);
     vTaskDelay( 100/portTICK_PERIOD_MS );
    }
  }
}

void Button_action( void * pvParameters)
{
  static uint8_t lastState = HIGH;
  uint8_t currentState;
  while(1)
  {
      currentState = digitalRead(BUTTON);
    if (currentState == LOW && lastState == HIGH)
     { 
          xSemaphoreGive( xSemaphore );
          Serial.println("Button pressed");
     }
     lastState=currentState;
      vTaskDelay( 10/portTICK_PERIOD_MS );
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xSemaphore = xSemaphoreCreateBinary();
  pinMode(LED,OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);
  xTaskCreate(toggle_led,"toggle_led",1024, NULL,2,&task1 ); 
  xTaskCreate(Button_action,"Button_action",1024, NULL,1,&task2 ); 
}

void loop() {
  // put your main code here, to run repeatedly:
}
