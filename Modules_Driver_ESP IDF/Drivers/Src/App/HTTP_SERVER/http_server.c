/*
 * http_server.c
 *
 *  Created on: Jan 14, 2026
 *      Author: Aya Ramadan
 */




#include "http_server.h"

#if HTTP_SERVER_ENABLED == STD_ON


static const char *TAG = "HTTP_SERVER";

static httpd_handle_t g_server = NULL;
static HttpServerConfig_t *g_cfg = NULL;

static httpd_ssl_config_t g_ssl_cfg;

/* parameter when https used to find cer & key */
extern const uint8_t cert_pem_start[] asm("_binary_cert_pem_start");
extern const uint8_t cert_pem_end[]   asm("_binary_cert_pem_end");

extern const uint8_t key_pem_start[] asm("_binary_key_pem_start");
extern const uint8_t key_pem_end[]   asm("_binary_key_pem_end");


/* ===================== Init ===================== */
void HttpServer_Init(HttpServerConfig_t *config)
{
    if (!config || config->endpoint_count == 0)
     {
	   #if HTTP_SERVER_DEBUG_ENABLED == STD_ON
        ESP_LOGE(TAG, "Invalid config or no endpoints defined");
       #endif
        return ;
     }

    g_cfg = config;
    
    #if HTTP_SERVER_DEBUG_ENABLED == STD_ON
      #if HTTP_SERVER_USE_SSL == 0
          ESP_LOGI(TAG, "HTTP Server initialized");
       #else 
          ESP_LOGI(TAG, "HTTPs Server initialized");
       #endif
    #endif
}
/* ===================== Start ===================== */
esp_err_t HttpServer_Start(void)
{
    if (!g_cfg) return ESP_ERR_INVALID_STATE;
    
       
   #if HTTP_SERVER_USE_SSL == 1
   
	/* ---------- HTTPS ---------- */
	  #if HTTP_SERVER_DEBUG_ENABLED == STD_ON
        ESP_LOGI(TAG, "Starting HTTPS server");
      #endif
      
      g_ssl_cfg = (httpd_ssl_config_t) HTTPD_SSL_CONFIG_DEFAULT();

        g_ssl_cfg.transport_mode = HTTPD_SSL_TRANSPORT_SECURE;
        g_ssl_cfg.port_secure    = g_cfg->port;
        g_ssl_cfg.httpd.max_uri_handlers = g_cfg->endpoint_count;
        
        g_ssl_cfg.servercert     = cert_pem_start;
        g_ssl_cfg.servercert_len = cert_pem_end - cert_pem_start;
        
        g_ssl_cfg.prvtkey_pem    = key_pem_start;
        g_ssl_cfg.prvtkey_len    = key_pem_end - key_pem_start;
        



        esp_err_t ret = httpd_ssl_start(&g_server, &g_ssl_cfg);
        if (ret != ESP_OK) 
        {
		  #if HTTP_SERVER_DEBUG_ENABLED == STD_ON
            ESP_LOGE(TAG, "Failed to start HTTPS server");
           #endif
            return ret;
         }
    #else
		  /* ---------- HTTP ---------- */

	    #if HTTP_SERVER_DEBUG_ENABLED == STD_ON
        ESP_LOGI(TAG, "Starting HTTP server");
        #endif
        
        httpd_config_t http_cfg = HTTPD_DEFAULT_CONFIG();
        http_cfg.server_port = g_cfg->port;
        http_cfg.max_uri_handlers = g_cfg->endpoint_count;

        esp_err_t ret = httpd_start(&g_server, &http_cfg);
        if (ret != ESP_OK) 
        {
			#if HTTP_SERVER_DEBUG_ENABLED == STD_ON
            ESP_LOGE(TAG, "Failed to start HTTP server");
            #endif
            return ret;
        }
   #endif

    /* Register endpoints */
    for (uint8_t i = 0; i < g_cfg->endpoint_count; i++) {
        httpd_uri_t uri = {
            .uri      = g_cfg->endpoints[i].uri,
            .method   = g_cfg->endpoints[i].method,
            .handler  = g_cfg->endpoints[i].handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(g_server, &uri);
    }

   #if HTTP_SERVER_DEBUG_ENABLED == STD_ON
    ESP_LOGI(TAG, "Server running on port %d", g_cfg->port);
    #endif
    return ESP_OK;
}
    
 

/* ===================== Stop ===================== */
esp_err_t HttpServer_Stop(void)
{
    if (g_server) {
        httpd_stop(g_server);
        g_server = NULL;
        #if HTTP_SERVER_DEBUG_ENABLED == STD_ON
        ESP_LOGI(TAG, "Server stopped");
        #endif
        return ESP_OK;
    }
    return ESP_ERR_INVALID_STATE;
}

#endif
