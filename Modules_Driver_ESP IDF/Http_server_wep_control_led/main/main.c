#include "../Src/Cfg.h"
/*-------- Hal Drivers -----------------*/
#include "../Src/Hal/GPIO/gpio.h"
#include "../Src/Hal/NVS/Nvs.h"
#include "../Src/Hal/WIFI/wifi.h"

/*-------- App Drivers -----------------*/

#include "../Src/App/HTTP_SERVER/http_server.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "hal/gpio_types.h"
#include "freertos/FreeRTOS.h"

/**
* connect esp And LAb or mobile with the same network
* open page by the esp ip to control
*
*/
static const char *TAG = "APP";

#define LED  2
static bool led_state = false;

/*-------- web page---------------*/
static const char *html_page =
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>ESP32 Server LED Control</title>"
"<style>"
"body { font-family: Arial; text-align:center; margin-top:50px; }"
"button { font-size:20px; padding:10px 20px; margin:10px; }"
"</style>"
"</head>"
"<body>"
"<h1>ESP32 LED Control</h1>"
"<p id='status'>LED Status: OFF</p>"
"<button onclick=\"fetch('/led/on')\">ON</button>"
"<button onclick=\"fetch('/led/off')\">OFF</button>"

"<script>"
"setInterval(() => {"
" fetch('/status').then(r => r.text()).then(t => {"
"   document.getElementById('status').innerHTML = 'LED Status: ' + t;"
" });"
"}, 1000);"
"</script>"

"</body>"
"</html>";


/* ===================== HTTP server(http,https) Handlers ===================== */
static esp_err_t root_handler(httpd_req_t *req)
{
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, " from root handler\n");
    return ESP_OK;
}

static esp_err_t led_on_handler(httpd_req_t *req)
{
    Gpio_WritePinValue( LED, 1);
    led_state = true;
    httpd_resp_send(req, "ON", HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, " from led_on_handler\n");
    return ESP_OK;
}

static esp_err_t led_off_handler(httpd_req_t *req)
{
    Gpio_WritePinValue(LED, 0);
    led_state = false;
    httpd_resp_send(req, "OFF", HTTPD_RESP_USE_STRLEN);
     ESP_LOGI(TAG, " from led_off_handler\n");
    return ESP_OK;
}

static esp_err_t status_handler(httpd_req_t *req)
{
    if (led_state)
        httpd_resp_send(req, "ON", HTTPD_RESP_USE_STRLEN);
    else
        httpd_resp_send(req, "OFF", HTTPD_RESP_USE_STRLEN);

 ESP_LOGI(TAG, " from status_handler\n");
    return ESP_OK;
}

void app_main(void)
{
//============== Types ============
  
  	Gpio_ConfigType led =
  	{
	  LED,
  	GPIO_MODE_INPUT_OUTPUT,
  	0,
  	GPIO_PULLDOWN_ONLY
  	};
  	

       //------Wifi data type -------- 
	//wifi_sta_config_app_t STA_cfg ={WIFI_STA_SSID,WIFI_STA_PASS};
	wifi_ap_config_app_t AP_Cfg ={WIFI_AP_SSID,WIFI_AP_PASS,4};
	wifi_driver_cfg_t cfg = 
	{
      .mode =     WIFI_APP_MODE_AP,
      .cred_src = WIFI_CRED_SRC_HARDCODED,
      .sta_cfg = NULL,
      .ap_cfg  = &AP_Cfg
   };

//============== Init ============
    Gpio_InitPin(&led);
    
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
      
    //============== http server inti ============
    
    /* Endpoints */
    static ServerEndpoint_t endpoints[] = 
    {
      {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_handler
     },

     {
        .uri = "/led/on",
        .method = HTTP_GET,
        .handler = led_on_handler
     },

     {
        .uri = "/led/off",
        .method = HTTP_GET,
        .handler = led_off_handler
     },

     {
        .uri = "/status",
        .method = HTTP_GET,
        .handler = status_handler
     }
    };
      /* Server config (HTTPs only) */
    static HttpServerConfig_t server_cfg =
     {
        .use_https = HTTP_SERVER_USE_SSL,
        .port = HTTP_SERVER_PORT,
        .endpoints = endpoints,
        .endpoint_count = 4
     };

    HttpServer_Init(&server_cfg);
    HttpServer_Start();

 //============== loop ============
    while (true) 
    {
        printf("Hello from Web page control \n");
        vTaskDelay(100);
    }
}
