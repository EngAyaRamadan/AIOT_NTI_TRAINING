/*
 * http_server.h
 *
 *  Created on: Jan 14, 2026
 *      Author: Aya Ramadan
 */

#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "../../Cfg.h"

#if HTTP_SERVER_ENABLED == STD_ON

#include "esp_http_server.h"
#include "esp_https_server.h"

#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"
/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif

/* ===================== Types ===================== */

typedef esp_err_t (*ServerHandler_t)(httpd_req_t *req);

typedef struct {
    const char       *uri;
    httpd_method_t    method;
    ServerHandler_t   handler;
} ServerEndpoint_t;


typedef struct {
    uint16_t port;
    bool use_https;
    
    /* HTTPS only */
    const char *cert_pem;
    const char *key_pem;

    ServerEndpoint_t *endpoints;
    uint8_t endpoint_count;
} HttpServerConfig_t;


void HttpServer_Init(HttpServerConfig_t *config);
esp_err_t HttpServer_Start(void);
esp_err_t HttpServer_Stop(void);



#ifdef __cplusplus
}
#endif

#endif // HTTP_ENABLED
#endif /* HTTP_SERVER_H_ */
