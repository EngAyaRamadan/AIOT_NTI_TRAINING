/*
 * http.h
 *
 *  Created on: Jan 13, 2026
 *      Author: Aya Ramadan
 */
/*
*  Driver support -> Http client
*                 -> https client  
*/
#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include "../../Cfg.h"

#if HTTP_CLIENT_ENABLED == STD_ON
#include "esp_http_client.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    HTTPCLIENT_METHOD_GET,
    HTTPCLIENT_METHOD_POST,
    HTTPCLIENT_METHOD_PUT,
    HTTPCLIENT_METHOD_DELETE
} HttpClientMethod_t;


typedef struct {
    const char *url;           // URL 
    HttpClientMethod_t method;       // Method
    const char *payload;       // NULL if-> GET
    const char *content_type;  // application/json
    uint32_t timeout_ms;       // Timeout
    const char *cert_pem;      // NULL -> use bundle, else use certificate
} HttpRequest_t;

typedef struct {
    int status_code;   // HTTP Response code
    char *response;    //  Response body
    int response_len;  //  Response length
} HttpResponse_t;

/* for retry mechanism*/
typedef struct {
    uint8_t max_retry;
    uint32_t retry_delay_ms;
} HttpRetryConfig_t;


/* -------------APIs------------------ */
void HttpClient_Init(void);
/* Send Request */
esp_err_t HttpClient_SendRequest(HttpRequest_t *req,HttpResponse_t *resp);

esp_err_t HttpClient_SendRequestWithRetry(HttpRequest_t *req,HttpResponse_t *resp,HttpRetryConfig_t *retry_cfg);

/* Free response buffer */
void HttpClient_FreeResponse(HttpResponse_t *resp);


#ifdef __cplusplus
}
#endif


#endif // HTTP_ENABLED

#endif /* HTTP_H_ */
