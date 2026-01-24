/*
 * mqtt.c
 *
 *  Created on: Jan 15, 2026
 *      Author: Aya Ramadan
 */


#include "mqtt.h"

#if MQTT_ENABLED == STD_ON


static const char *g_TAG = TAG_MQTT;

static esp_mqtt_client_handle_t client = NULL;
static mqtt_event_cb_t app_cb = NULL;

extern const char hivemq_root_ca_pem[] asm("_binary_mqtt_cer_pem_start");

/* ================== EVENT HANDLER ================== */
static void mqtt_event_handler(void *arg,esp_event_base_t base,int32_t event_id,void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch (event_id) {

      case MQTT_EVENT_CONNECTED:
        #if MQTT_DEBUG_ENABLED  == STD_ON
          ESP_LOGI(g_TAG, "Connected");
        #endif 
          if (app_cb)
              app_cb(MQTT_EVT_CONNECTED, NULL,0, NULL, 0);
          break;
      
      case MQTT_EVENT_DISCONNECTED:
         #if MQTT_DEBUG_ENABLED  == STD_ON
          ESP_LOGW(g_TAG, "Disconnected");
         #endif 
          if (app_cb)
              app_cb(MQTT_EVT_DISCONNECTED, NULL, 0,NULL, 0);
          break;
      
      case MQTT_EVENT_DATA:
          #if MQTT_DEBUG_ENABLED  == STD_ON
           ESP_LOGI(g_TAG, "receive data");
          #endif 
          if (app_cb)
              app_cb(MQTT_EVT_DATA,event->topic,event->topic_len,event->data,event->data_len);
          break;
      
      case MQTT_EVENT_ERROR:
         #if MQTT_DEBUG_ENABLED  == STD_ON
          ESP_LOGE(g_TAG, "MQTT Error");
         #endif 
          if (app_cb)
              app_cb(MQTT_EVT_ERROR, NULL, 0,NULL, 0);
          break;
      
      default:
          break;
    }
}

/* ================== INIT ================== */
void mqtt_driver_init(const mqtt_config_t *cfg)
{
     app_cb = cfg->cb;

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = cfg->uri,
        .credentials.client_id = cfg->client_id,
    };

    /* -------- Authentication -------- */
    #if MQTT_USE_AUTHANTICATION == 1
        mqtt_cfg.credentials.username = cfg->username;
        mqtt_cfg.credentials.authentication.password = cfg->password;
    #endif
    /* -------- TLS -------- */
   #if MQTT_USE_TLS == 1 
        mqtt_cfg.broker.verification.certificate = hivemq_root_ca_pem;
   #endif

    client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(client,ESP_EVENT_ANY_ID,mqtt_event_handler,NULL);

    esp_mqtt_client_start(client);
    
    #if HTTP_SERVER_ENABLED  == STD_ON
      #if MQTT_USE_TLS == 1 
          ESP_LOGI(g_TAG, "MQTTs Init");
       #else   
          ESP_LOGI(g_TAG, "MQTT Init");
      #endif    
    #endif 
}

/* ================== APIs ================== */
void mqtt_driver_publish(const char *topic,const char *payload,int qos,bool retain)
{
    if (!client)
        return ;

    esp_mqtt_client_publish(client, topic, payload, 0, qos, retain) ;
    
     #if MQTT_DEBUG_ENABLED  == STD_ON
        ESP_LOGI(g_TAG, "MQTT Publish:  %s",payload);
     #endif 
}

void mqtt_driver_subscribe(const char *topic, int qos)
{
    if (!client)
        return ;

  esp_mqtt_client_subscribe(client, topic, qos) ;
  
  #if MQTT_DEBUG_ENABLED  == STD_ON
     ESP_LOGI(g_TAG, "MQTT Subscribe on:  %s",topic);
  #endif 
}

void mqtt_driver_disconnect(void)
{
    if (client)
        esp_mqtt_client_stop(client);
       
  #if MQTT_DEBUG_ENABLED  == STD_ON
     ESP_LOGI(g_TAG, "MQTT disconnect");
  #endif    
}


#endif 