/*
 * wifi.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Aya Ramadan
 */

#include "wifi.h"

#if WIFI_ENABLED == STD_ON

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "../NVS/Nvs.h"

static const char *g_Tag = TAG_WIFI;

typedef struct
{
    char ssid[32];
    char password[64];
    bool valid;
} wifi_cred_t;

/* ===================== STATIC DATA ===================== */
static wifi_app_mode_t g_app_mode ;
static wifi_state_t g_wifi_state = WIFI_STATE_IDLE;
static uint8_t g_retry_count = 0;

/* ===================== FORWARD DECL ===================== */
static void wifi_sm_handle_event(wifi_driver_event_t evt);


static void wifi_event_handler(void *arg,esp_event_base_t event_base,int32_t event_id,void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
             wifi_sm_handle_event(WIFI_STA_START);
            #if WIFI_DEBUG_ENABLED == STD_ON
             ESP_LOGI(g_Tag, "WiFi STA Mode start");
            #endif
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            wifi_sm_handle_event(WIFI_STA_DISCONNECTED);
            #if WIFI_DEBUG_ENABLED == STD_ON
             ESP_LOGI(g_Tag, "WiFi STA Mode disconnect");
            #endif
            break;

        case WIFI_EVENT_AP_START:
        wifi_sm_handle_event(WIFI_AP_STARTED);
           #if WIFI_DEBUG_ENABLED == STD_ON
             ESP_LOGI(g_Tag, "WiFi AP Mode start");
           #endif
           break;

        case WIFI_EVENT_AP_STOP:
        wifi_sm_handle_event(WIFI_AP_STOPPED);
           #if WIFI_DEBUG_ENABLED == STD_ON
             ESP_LOGI(g_Tag, "WiFi AP Mode stop");
            #endif
            break;
            
        default:
            break;            
        }
    }
    else if (event_base == IP_EVENT &&
             event_id == IP_EVENT_STA_GOT_IP)
    {
       wifi_sm_handle_event(WIFI_STA_CONNECTED);
           #if WIFI_DEBUG_ENABLED == STD_ON
             ESP_LOGI(g_Tag, "WiFi IP STA Got");
            #endif

    }
}

/* ===================== STATE MACHINE ===================== */
static void wifi_sm_handle_event(wifi_driver_event_t evt)
{
    switch (g_wifi_state)
    {
    case WIFI_STATE_STA_CONNECTING:

        if (evt == WIFI_STA_START ||
            evt == WIFI_STA_DISCONNECTED)
        {
            if (++g_retry_count <= WIFI_MAX_RETRY)
            {
				esp_wifi_connect();
				#if WIFI_DEBUG_ENABLED == STD_ON
                ESP_LOGI(g_Tag, "STA retry %d", g_retry_count);
                 #endif
                
            }
            else
            {
			  if (g_app_mode == WIFI_APP_MODE_AP_STA)
                 {
                     ESP_LOGW(g_Tag, "STA failed → stay AP only (Dual mode)");
                     g_wifi_state = WIFI_STATE_AP_RUNNING;
                      #if WIFI_DEBUG_ENABLED == STD_ON
                         ESP_LOGI(g_Tag, "STA failed → switch to AP");
                      #endif
                 }
             else
                 {
					  g_wifi_state = WIFI_STATE_ERROR;
					  #if WIFI_DEBUG_ENABLED == STD_ON
                        ESP_LOGI(g_Tag, "STA failed (STA only mode)");
                      #endif
                 }   
               
                

            }
        }
        else if (evt == WIFI_STA_CONNECTED)
        {
            
            g_wifi_state = WIFI_STATE_STA_CONNECTED;
            g_retry_count = 0;
            
            #if WIFI_DEBUG_ENABLED == STD_ON
              ESP_LOGI(g_Tag, "STA connected");
            #endif
        }
        break;

    case WIFI_STATE_STA_CONNECTED:

        if (evt == WIFI_STA_DISCONNECTED)
        {
            
            g_wifi_state = WIFI_STATE_STA_CONNECTING;
            g_retry_count = 0;
            esp_wifi_connect();
            
            #if WIFI_DEBUG_ENABLED == STD_ON
              ESP_LOGI(g_Tag, "STA lost connection");
            #endif
        }
        break;

    case WIFI_STATE_AP_RUNNING:
        /* AP only – nothing special for now */
        #if WIFI_DEBUG_ENABLED == STD_ON
          ESP_LOGI(g_Tag, "AP_RUNNING");
        #endif
        break;

    default:
        break;
    }
}
/* ===================== Init func ===================== */
void wifi_driver_init(void)
{
	// Initialize network interface
    esp_netif_init();
    // Create default event loop
    esp_event_loop_create_default();
    
     // Create default Wi-Fi station
    esp_netif_create_default_wifi_sta();
    // Create default Wi-Fi access point
    esp_netif_create_default_wifi_ap();

    // Default Wi-Fi config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    // Register event handlers
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);


    #if WIFI_DEBUG_ENABLED == STD_ON
        ESP_LOGI(g_Tag, "WiFi driver initialized");
     #endif
}

/* ===================== Load STA cred (hard , NVS) ===================== */
static wifi_cred_t wifi_load_sta_cred(const wifi_driver_cfg_t *cfg)
{
    wifi_cred_t cred = {0};

    if (cfg->cred_src == WIFI_CRED_SRC_HARDCODED)
    {
        if (cfg->sta_cfg)
        {
            strcpy(cred.ssid, cfg->sta_cfg->ssid);
            strcpy(cred.password, cfg->sta_cfg->password);
            cred.valid = true;
        }
    }
    else
    {
        size_t len;

        len = sizeof(cred.ssid);
        if (nvs_driver_get_str("wifi", "sta_ssid", cred.ssid, &len) != ESP_OK)
            return cred;

        len = sizeof(cred.password);
        if (nvs_driver_get_str("wifi", "sta_pass", cred.password, &len) != ESP_OK)
            return cred;

        cred.valid = true;
    }

    return cred;
}
/* ===================== Load AP cred (hard , NVS) ===================== */
static wifi_cred_t wifi_load_ap_cred(const wifi_driver_cfg_t *cfg)
{
    wifi_cred_t cred = {0};

    if (cfg->cred_src == WIFI_CRED_SRC_HARDCODED)
    {
        if (cfg->ap_cfg)
        {
            strcpy(cred.ssid, cfg->ap_cfg->ssid);
            strcpy(cred.password, cfg->ap_cfg->password);
            cred.valid = true;
        }
    }
    else
    {
        size_t len;

        len = sizeof(cred.ssid);
        if (nvs_driver_get_str("wifi", "ap_ssid", cred.ssid, &len) != ESP_OK)
            return cred;

        len = sizeof(cred.password);
        if (nvs_driver_get_str("wifi", "ap_pass", cred.password, &len) != ESP_OK)
            return cred;

        cred.valid = true;
    }

    return cred;
}


/* ===================== Start function ===================== */
void wifi_driver_start(const wifi_driver_cfg_t *cfg)
{
    wifi_config_t esp_sta_cfg = {0};
    wifi_config_t esp_ap_cfg  = {0};

    g_app_mode   = cfg->mode;
    g_retry_count = 0;
    
    wifi_cred_t sta;
     wifi_cred_t ap ;
       
    if(g_app_mode == WIFI_APP_MODE_STA || g_app_mode == WIFI_APP_MODE_AP_STA)
     {
		 sta = wifi_load_sta_cred(cfg);
		 if (sta.valid)
          {
              strcpy((char *)esp_sta_cfg.sta.ssid, sta.ssid);
              strcpy((char *)esp_sta_cfg.sta.password, sta.password);
              #if WIFI_DEBUG_ENABLED == STD_ON
                 ESP_LOGI(g_Tag, "WiFi STA cred valid");
              #endif
          }
      } 
      
      if (g_app_mode == WIFI_APP_MODE_AP || g_app_mode == WIFI_APP_MODE_AP_STA)
       {
		    ap  = wifi_load_ap_cred(cfg);
		    if (ap.valid)
               {
                   strcpy((char *)esp_ap_cfg.ap.ssid, ap.ssid);
                   strcpy((char *)esp_ap_cfg.ap.password, ap.password);
              
                   esp_ap_cfg.ap.max_connection =
                       cfg->ap_cfg ? cfg->ap_cfg->max_conn : 4;
              
                   
                   if (strlen(ap.password) < 8)
                       {
              				esp_ap_cfg.ap.authmode = WIFI_AUTH_OPEN;
              			    #if WIFI_DEBUG_ENABLED == STD_ON
                            ESP_LOGI(g_Tag, "WiFi AP pass less then 8 char so WIFI_AUTH_OPEN");
                           #endif
                       }
                   else
                       {
              				esp_ap_cfg.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
              				#if WIFI_DEBUG_ENABLED == STD_ON
                            ESP_LOGI(g_Tag, "WiFi AP pass greater then 8 char so WIFI_AUTH_WPA_WPA2_PSK");
                           #endif
                       }
                       
                       
                    #if WIFI_DEBUG_ENABLED == STD_ON
                        ESP_LOGI(g_Tag, "WiFi AP cred valid");
                     #endif
                       
               }
	   }
    
    switch (g_app_mode)
    {
    case WIFI_APP_MODE_STA:
        g_wifi_state = WIFI_STATE_STA_CONNECTING;
        if (!sta.valid) 
        {
			  #if WIFI_DEBUG_ENABLED == STD_ON
               ESP_LOGW(g_Tag, "WiFi STA No cred valid");
              #endif			
			return;
	    }
        esp_wifi_set_mode(WIFI_MODE_STA);
        esp_wifi_set_config(WIFI_IF_STA, &esp_sta_cfg);
     #if WIFI_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "WiFi STA start");
     #endif  
        break;

    case WIFI_APP_MODE_AP:
         if (!ap.valid)
         {
			  #if WIFI_DEBUG_ENABLED == STD_ON
               ESP_LOGW(g_Tag, "WiFi AP No cred valid");
              #endif
			 return;
		 } 
        g_wifi_state = WIFI_STATE_AP_RUNNING;
        esp_wifi_set_mode(WIFI_MODE_AP);
        esp_wifi_set_config(WIFI_IF_AP, &esp_ap_cfg);
     #if WIFI_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "WiFi AP start");
     #endif  
        break;

    case WIFI_APP_MODE_AP_STA:
       
    
        if (sta.valid)
           {
			    g_wifi_state = WIFI_STATE_STA_CONNECTING;
			    
			   esp_wifi_set_mode(WIFI_MODE_APSTA);
			   esp_wifi_set_config(WIFI_IF_STA, &esp_sta_cfg);
           }
         else
           {
			           /* AP only fallback */
               g_wifi_state = WIFI_STATE_AP_RUNNING;

               esp_wifi_set_mode(WIFI_MODE_AP);

			   #if WIFI_DEBUG_ENABLED == STD_ON
                ESP_LOGW(g_Tag, "WiFi STA NO cred valid");
              #endif
            }
        if (ap.valid)
           esp_wifi_set_config(WIFI_IF_AP, &esp_ap_cfg);
         else
           #if WIFI_DEBUG_ENABLED == STD_ON
               ESP_LOGW(g_Tag, "WiFi AP No cred valid");
             #endif  
           
     #if WIFI_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "WiFi Dual mode start");
     #endif   
        break;
    }

    esp_wifi_start();
}
/* ===================== stop function ===================== */
void wifi_driver_stop(void)
{
    esp_wifi_stop();
    g_wifi_state = WIFI_STATE_IDLE;
     #if WIFI_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "WiFi stopped");
     #endif    
}



#endif