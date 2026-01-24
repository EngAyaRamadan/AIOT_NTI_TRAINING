#include "../Src/Cfg.h"
/*-------- Hal Drivers -----------------*/
#include "../Src/Hal/GPIO/gpio.h"
#include "../Src/Hal/NVS/Nvs.h"
#include "../Src/Hal/WIFI/wifi.h"
#include "../Src/Hal/UART/uart.h"
#include "../Src/Hal/I2C/i2c.h"
/*-------- App Drivers -----------------*/
#include "../Src/App/POT/pot.h"
#include "../Src/App/ULTRASONIC/ultrasonic.h"
#include "../Src/App/HTTP_CLIENT/http_client.h"
#include "../Src/App/HTTP_SERVER/http_server.h"
#include "../Src/App/MQTT/mqtt.h"

#include "esp_wifi.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "hal/gpio_types.h"
#include "freertos/FreeRTOS.h"



#define LED  2
static const char *TAG = "APP";

/* ===================== HTTP server(http,https) Handlers ===================== */
/* GET http://ESP_IP/hello */
static esp_err_t hello_handler(httpd_req_t *req)
{
    const char *resp = "Hello HTTP Server Driver from esp!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "From get hello server");
    return ESP_OK;
}
/* POST http://ESP_IP/echo */
static esp_err_t echo_handler(httpd_req_t *req)
{
    char buf[128];
    int len = httpd_req_recv(req, buf, sizeof(buf));

    ESP_LOGI(TAG, "Received %d bytes", len);

    httpd_resp_send(req, buf, len);
    return ESP_OK;
}

/*
void Uart_Task(void *pvParameters)
{
    uart_event_t event;
    uint8_t rx_buffer[128];

    QueueHandle_t uart_queue = Uart_GetEventQueue(UART_NUM_1);

    while (1)
    {
        if (xQueueReceive(uart_queue, &event, portMAX_DELAY))
        {
            switch (event.type)
            {
                case UART_DATA:
                {
                    int len = Uart_Read(UART_NUM_1, rx_buffer, event.size,10);
                    rx_buffer[len] = '\0';
                    printf( "RX: %s\n", rx_buffer);
                    break;
                }

                case UART_FIFO_OVF:
                case UART_BUFFER_FULL:
                    printf( "UART overflow");
                    uart_flush_input(UART_NUM_1);
                    xQueueReset(uart_queue);
                    break;

                default:
                    break;
            }
        }
    }
}
*/

void mqtt_cb(mqtt_event_t evt,
             const char *topic,
             int topic_len,
             const char *data,
             int data_len)
{
    switch (evt) {

    case MQTT_EVT_CONNECTED:
        ESP_LOGI("APP", "MQTT Connected");

        mqtt_driver_subscribe("aya/control", 0);
        mqtt_driver_publish("aya/test", "HELLO_FROM_ESP", 0, false);
        break;

    case MQTT_EVT_DATA:
        ESP_LOGI("APP", "RX Topic=%.*s Data=%.*s",topic_len, topic,data_len, data);

       char msg[MQTT_MAX_PAYLOAD];
       int len = data_len;
       if (len >= MQTT_MAX_PAYLOAD)
           len = MQTT_MAX_PAYLOAD - 1;
 
       memcpy(msg, data, len);
       msg[len] = '\0';
        if(!strcmp(msg,"ON"))
        {
			Gpio_WritePinValue(LED,1);
			vTaskDelay(100);
			Gpio_WritePinValue(LED,0);
		}

        break;

    case MQTT_EVT_DISCONNECTED:
        ESP_LOGW("APP", "MQTT Disconnected");
        break;

    default:
        break;
    }
}


void app_main(void)
{
//============== Types ============
	int pot_value=0;
	float ds;
	uint8_t rx_data[10];
	int rx_len=0;
  
  	Gpio_ConfigType led =
  	{
	  LED,
  	GPIO_MODE_INPUT_OUTPUT,
  	0,
  	GPIO_PULLDOWN_ONLY
  	};
  	
	Ultrasonic_Config_t Ultrasonic =
	{
		TRIGGER_GPIO_PIN,
		ECHO_GPIO_PIN
	};
	
    Uart_ConfigType uart1 =
    {
        .uart_num = UART_NUM_1,
        .baud_rate = UART_BUAD_RATE,
        .tx_pin = GPIO_NUM_17,
        .rx_pin = GPIO_NUM_16
    };


       //------Wifi data type -------- 
	wifi_sta_config_app_t STA_cfg ={WIFI_STA_SSID,WIFI_STA_PASS};
	//wifi_ap_config_app_t AP_Cfg ={WIFI_AP_SSID,WIFI_AP_PASS,4};
	wifi_driver_cfg_t cfg = 
	{
      .mode =     WIFI_APP_MODE_STA,
      .cred_src = WIFI_CRED_SRC_HARDCODED,
      .sta_cfg = &STA_cfg,
      .ap_cfg  = NULL
   };

//============== Init ============
    Gpio_InitPin(&led);
    Pot_Init();
    Ultrasonic_Init(&Ultrasonic);

    Uart_Init(&uart1);

   // xTaskCreate(Uart_Task,"Uart_Task",4096,NULL,10,NULL);
            
     //-------- Wifi inti -----          
	nvs_driver_init();
    /*	
	  nvs_driver_set_str(WIFI_NVS_NAMESPACE, WIFI_NVS_STA_SSID, "Aya Ramadan");
      nvs_driver_set_str(WIFI_NVS_NAMESPACE, WIFI_NVS_STA_PASS, "ayaramadan22");
    // nvs_driver_erase_all(WIFI_NVS_NAMESPACE);
      nvs_driver_set_str(WIFI_NVS_NAMESPACE, WIFI_NVS_AP_SSID, "ESP32_Config");
      nvs_driver_set_str(WIFI_NVS_NAMESPACE, WIFI_NVS_AP_PASS, "");            
	*/
	
      wifi_driver_init();
      wifi_driver_start(&cfg);
      
      //------- http client inti-------- 
      HttpClient_Init();
      HttpResponse_t resp;
      
      HttpRetryConfig_t retry_cfg = 
      {
        .max_retry = MAX_RETRY,
        .retry_delay_ms = RETRY_DELAY_MS
      };

    //============== http server inti ============
    /* Endpoints */
    static ServerEndpoint_t endpoints[] = 
    {
        {
            .uri = "/hello",
            .method = HTTP_GET,
            .handler = hello_handler
        },
        {
            .uri = "/echo",
            .method = HTTP_POST,
            .handler = echo_handler
        }
    };
      /* Server config (HTTPs only) */
    static HttpServerConfig_t server_cfg =
     {
        .use_https = HTTP_SERVER_USE_SSL,
        .port = HTTP_SERVER_PORT,
        .endpoints = endpoints,
        .endpoint_count = 2
     };

    HttpServer_Init(&server_cfg);
    HttpServer_Start();



mqtt_config_t mqtt_cfg = {
    .uri       = MQTT_BROKER_URI,
    .client_id = MQTT_CLIENT_ID,
    .cb        = mqtt_cb,
    .username  = MQTT_USERNAME,
    .password  = MQTT_PASSWORD,
    .cert_pem  = MQTT_CERT_PEM
};

mqtt_driver_init(&mqtt_cfg);
 //============== loop ============
    while (true) 
    {
        printf("Hello from Aya drivers and http server and client !\n");
       // Gpio_TogglePinValue(LED);
        
         //============== POT test ============
        Pot_ReadRaw(POT1_CHANNEL ,&pot_value);
        Pot_ReadMap(POT1_CHANNEL , &pot_value,POT1_MIN_VALUE , POT1_MAX_VALUE);
       
         //============== Ultrasonic test ============
        Ultrasonic_Main(&ds);
	    printf("ULS Value = %f\n",ds); 
	
         //============== UART_Polling ============
        Uart_Write(UART_NUM_1, (const uint8_t *)"Aya ramadan", strlen("Aya"));
        rx_len=Uart_Read(UART_NUM_1, rx_data,15,10);
         rx_data[rx_len]='\0';
         printf("Data receive: %s\n",rx_data);
         
         
         //============== Http client  ============
         char payload[128];
        snprintf(payload, sizeof(payload),
                "{\"device\":\"ESP32\",\"pot\":%d ,\"Distance\":%f ,\"Receive data\":%s}", pot_value,ds,rx_data);

         HttpRequest_t req = 
         {
            .url = HTTP_CLIENT_URL,
            .method = HTTP_CLIENT_METHOD,
            .payload = payload,
            .content_type = HTTP_CLIENT_CONTENET_TYPE,
            .timeout_ms = HTTP_CLIENT_TIMEOUT_MS,
            .cert_pem = NULL
        };
       esp_err_t err = HttpClient_SendRequest(&req, &resp);
       err = HttpClient_SendRequestWithRetry(&req, &resp, &retry_cfg);

        if(err == ESP_OK)
         {
           ESP_LOGI(TAG, "Response:\n%s", resp.response);
         }

          HttpClient_FreeResponse(&resp);
          
        mqtt_driver_publish("aya/test", payload, 0, false);
        vTaskDelay(1000);
    }
}
