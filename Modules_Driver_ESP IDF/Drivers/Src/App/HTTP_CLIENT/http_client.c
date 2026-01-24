/*
 * http.c
 *
 *  Created on: Jan 13, 2026
 *      Author: Aya Ramadan
 */


#include "http_client.h"

#if HTTP_CLIENT_ENABLED == STD_ON
#include "esp_crt_bundle.h"


static const char *g_Tag = TAG_HTTP_CLIENT;

//==========================================================
/* Event handler for esp_http_client */
static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    HttpResponse_t *resp = (HttpResponse_t *)evt->user_data;

    switch(evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(evt->client)) {
                resp->response = realloc(resp->response,
                                         resp->response_len + evt->data_len + 1);
                memcpy(resp->response + resp->response_len, evt->data, evt->data_len);
                resp->response_len += evt->data_len;
                resp->response[resp->response_len] = '\0';
            }
            break;
        default:
            break;
    }

    return ESP_OK;
}
//==========================================================
void HttpClient_Init(void)
{  
	#if HTTP_CLIENT_DEBUG_ENABLED == STD_ON  
	  #if HTTP_CLIENT_USE_SSL == 0
         ESP_LOGI(g_Tag, "HTTP Client Driver Initialized");  
      #else 
         ESP_LOGI(g_Tag, "HTTPs Client Driver Initialized");
      #endif   
    #endif
}
//==========================================================
static esp_http_client_method_t convert_method(HttpClientMethod_t method)
{
    switch(method)
    {
        case HTTPCLIENT_METHOD_POST: return HTTP_METHOD_POST;
        case HTTPCLIENT_METHOD_PUT: return HTTP_METHOD_PUT;
        case HTTPCLIENT_METHOD_DELETE: return HTTP_METHOD_DELETE;
        default: return HTTP_METHOD_GET;
    }
}
//==========================================================
esp_err_t HttpClient_SendRequest(HttpRequest_t *req,HttpResponse_t *resp)
{

    memset(resp, 0, sizeof(HttpResponse_t));

    esp_http_client_config_t config = {
        .url = req->url,
        .method = convert_method(req->method),
        .event_handler = http_event_handler,
        .user_data = resp,
        .timeout_ms = req->timeout_ms,
    };

if (strncmp(req->url, "https://", 8) == 0) {
#if HTTP_CLIENT_USE_SSL == 1
    if(req->cert_pem)
     {
        config.cert_pem = req->cert_pem; 
        
        #if HTTP_CLIENT_DEBUG_ENABLED == STD_ON  
         ESP_LOGI(g_Tag, "HTTPs Certificate attach");
        #endif  
    } 
    else 
    {
        config.crt_bundle_attach = esp_crt_bundle_attach; 
        
        #if HTTP_CLIENT_DEBUG_ENABLED == STD_ON  
         ESP_LOGI(g_Tag, "HTTPs bundle attach");
        #endif  
    }
#endif
}

    esp_http_client_handle_t client = esp_http_client_init(&config);

    if(req->payload) {
        esp_http_client_set_header(client, "Content-Type", req->content_type);
        esp_http_client_set_post_field(client, req->payload, strlen(req->payload));
    }

    esp_err_t err = esp_http_client_perform(client);

    if(err == ESP_OK) 
    {
        resp->status_code = esp_http_client_get_status_code(client);
        
        #if HTTP_CLIENT_DEBUG_ENABLED == STD_ON
          ESP_LOGI(g_Tag, "HTTP Status = %d", resp->status_code);
        #endif
    } 
    else 
    {
		#if HTTP_CLIENT_DEBUG_ENABLED == STD_ON
          ESP_LOGE(g_Tag, "HTTP request failed: %s", esp_err_to_name(err));
        #endif
    }

    esp_http_client_cleanup(client);
  return err;
}

esp_err_t HttpClient_SendRequestWithRetry(HttpRequest_t *req,HttpResponse_t *resp,HttpRetryConfig_t *retry_cfg)
{
    esp_err_t err = ESP_FAIL;

    for(uint8_t attempt = 0; attempt <= retry_cfg->max_retry; attempt++)
    {
		#if HTTP_CLIENT_DEBUG_ENABLED == STD_ON
           ESP_LOGI(g_Tag, "HTTP Attempt %d", attempt + 1);
        #endif
        
        err = HttpClient_SendRequest(req, resp);

        if(err == ESP_OK && resp->status_code == 200)
        {
          #if HTTP_CLIENT_DEBUG_ENABLED == STD_ON
            ESP_LOGI(g_Tag, "HTTP Success");
          #endif  
            return ESP_OK;
        }
        #if HTTP_CLIENT_DEBUG_ENABLED == STD_ON
          ESP_LOGW(g_Tag, "HTTP Failed (err=%s, status=%d)",esp_err_to_name(err), resp->status_code);
        #endif
        
        HttpClient_FreeResponse(resp);

        if(attempt < retry_cfg->max_retry)
        {
            vTaskDelay(pdMS_TO_TICKS(retry_cfg->retry_delay_ms));
        }
    }
    return err;
}
//==========================================================
void HttpClient_FreeResponse(HttpResponse_t *resp)
{
    if(resp && resp->response) {
        free(resp->response);
        resp->response = NULL;
        resp->response_len = 0;
        #if HTTP_CLIENT_DEBUG_ENABLED == STD_ON
          ESP_LOGI(g_Tag, "HTTP Response free");
        #endif
    }
}

#endif