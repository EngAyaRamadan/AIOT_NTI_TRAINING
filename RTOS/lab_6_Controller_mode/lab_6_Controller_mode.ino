/*
  3- Events
*/
#include <Arduino.h>

#define BUTTON1_PIN 4
#define BUTTON2_PIN 5
#define BUTTON3_PIN 18
#define LED_PIN     2

#define Battery_bit  (1<<0)
#define temp_bit (1<<1)
#define Button_bit (1<<2)

EventGroupHandle_t xCreatedEventGroup;
EventBits_t uxBits;

void Battery_task( void * pvParameters)
{
  uint8_t laststate =HIGH;
  uint8_t currentstate ;
  while(1)
  {
    currentstate = digitalRead(BUTTON1_PIN);
    if (currentstate  == LOW && laststate== HIGH )
   {
      xEventGroupSetBits(xCreatedEventGroup,Battery_bit );
      Serial.println("button Battery set");
   }
   laststate = currentstate;
       vTaskDelay( 200/portTICK_PERIOD_MS );
  }
}
void Temp_Task( void * pvParameters)
{
  uint8_t laststate =HIGH;
  uint8_t currentstate ;
  while(1)
  {
    currentstate = digitalRead(BUTTON2_PIN);
   if ( currentstate == LOW && laststate ==HIGH )
   {
      xEventGroupSetBits(xCreatedEventGroup,temp_bit );
      Serial.println("button temp set");
   }
     laststate = currentstate;
       vTaskDelay( 200/portTICK_PERIOD_MS );
  }
 }
void Button_Task( void * pvParameters)
{
    uint8_t laststate =HIGH;
  uint8_t currentstate ;
  while(1)
  {
    currentstate = digitalRead(BUTTON3_PIN); 
     if ( currentstate == LOW && laststate ==HIGH )
   {
      xEventGroupSetBits(xCreatedEventGroup,Button_bit );
      Serial.println("button  set");
   }
    laststate = currentstate;
    vTaskDelay( 200/portTICK_PERIOD_MS );
  }
}

void Controller(void *pvParameters)
{
  while (1)
  {
    uxBits = xEventGroupWaitBits(
        xCreatedEventGroup,
        Battery_bit | temp_bit | Button_bit,
        pdTRUE,      // clear after reading
        pdFALSE,
        portMAX_DELAY);

    // -------- Case 1: Battery + Temp ONLY --------
    if ((uxBits & (Battery_bit | temp_bit | Button_bit)) == (Battery_bit | temp_bit))
    {
      Serial.println("Blink FAST");
      digitalWrite(LED_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(300));
      digitalWrite(LED_PIN, LOW);
    }

    // -------- Case 2: Battery OR Temp ONLY --------
    else if (
        (uxBits == Battery_bit) ||
        (uxBits == temp_bit))
    {
      Serial.println("Blink SLOW");
      digitalWrite(LED_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(1000));
      digitalWrite(LED_PIN, LOW);
    }

    // -------- Case 3: Button ONLY --------
    else if (uxBits == Button_bit)
    {
      Serial.println("LED STEADY ON");
      digitalWrite(LED_PIN, HIGH);
    }

    // -------- Default --------
    else
    {
      digitalWrite(LED_PIN, LOW);
    }

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

//void Controller( void * pvParameters)
//{
//  while(1)
//  {
//    Serial.println(" Controller Task");
//     uxBits=xEventGroupWaitBits(xCreatedEventGroup, Battery_bit | temp_bit |Button_bit,pdTRUE,pdFALSE,portMAX_DELAY);
//    if((uxBits  & ( Battery_bit | temp_bit |Button_bit ) )== (temp_bit |temp_bit |!(Button_bit)))
//    {
//      Serial.println(" blink fast");
//        digitalWrite(LED_PIN,HIGH);
//        vTaskDelay( 1000/portTICK_PERIOD_MS );
//        digitalWrite(LED_PIN,LOW);
//    }
//
//    else if ((uxBits  & ( Battery_bit | temp_bit |Button_bit )) == !(Button_bit) )
//    {
//         Serial.println(" blink slowly");
//        digitalWrite(LED_PIN,HIGH);
//        vTaskDelay( 2000/portTICK_PERIOD_MS );
//        digitalWrite(LED_PIN,LOW);
//    }
//    else if((uxBits  & ( Battery_bit | temp_bit |Button_bit )) == (Button_bit))
//    {
//         Serial.println(" LED Steady HIGH");
//         digitalWrite(LED_PIN,HIGH);   
//    }
//     vTaskDelay( 1000/portTICK_PERIOD_MS );
//  }
//}
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   pinMode(LED_PIN, OUTPUT);
   pinMode(BUTTON1_PIN, INPUT_PULLUP);
   pinMode(BUTTON2_PIN, INPUT_PULLUP);
   pinMode(BUTTON3_PIN, INPUT_PULLUP);
  xCreatedEventGroup = xEventGroupCreate();
  
  xTaskCreate(Battery_task,"Battery_task",1024, NULL,1,NULL ); 
  xTaskCreate(Temp_Task,"Temp_Task",1024, NULL,1,NULL ); 
  xTaskCreate(Button_Task,"Button_Task",1024, NULL,1,NULL );
  xTaskCreate(Controller,"Controller",1024, NULL,1,NULL ); 
}

void loop() {
  // put your main code here, to run repeatedly:
}
