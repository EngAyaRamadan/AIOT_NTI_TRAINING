/*
 * wifi.h
 *
 *  Created on: Jan 11, 2026
 *      Author: Aya Ramadan
 */
/* 
 * WIFI State machine  Driver support ->  STA ONLY
 *                                   OR   AP ONLY
 *                                   OR   Dual_Mode    
 *
 *  support -> Credential Hard or in NVS 
 * 
*/
#ifndef WIFI_H_
#define WIFI_H_

#include "../../Cfg.h"


#if WIFI_ENABLED == STD_ON

#include <stdint.h>
#include "string.h"
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

/* in app must init NVS*/

/* ===================== Types ===================== */
typedef struct
{
    char ssid[32];
    char password[64];
} wifi_sta_config_app_t;

typedef struct
{
    char ssid[32];
    char password[64];
    uint8_t max_conn;
} wifi_ap_config_app_t;

typedef enum
{
    WIFI_CRED_SRC_HARDCODED = 0,
    WIFI_CRED_SRC_NVS
} wifi_cred_source_t;


typedef enum
{
    WIFI_APP_MODE_STA = 0,
    WIFI_APP_MODE_AP,
    WIFI_APP_MODE_AP_STA
} wifi_app_mode_t;

typedef struct
{
    wifi_app_mode_t     mode;
    wifi_cred_source_t cred_src;

    /* Used only if HARDCODED */
    wifi_sta_config_app_t *sta_cfg;
    wifi_ap_config_app_t  *ap_cfg;
} wifi_driver_cfg_t;


typedef enum
{
	WIFI_STA_START,
    WIFI_STA_CONNECTED,
    WIFI_STA_DISCONNECTED,
    WIFI_AP_STARTED,
    WIFI_AP_STOPPED
} wifi_driver_event_t;

typedef enum
{
    WIFI_STATE_IDLE = 0,
    WIFI_STATE_STA_CONNECTING,
    WIFI_STATE_STA_CONNECTED,
    WIFI_STATE_AP_RUNNING,
    WIFI_STATE_ERROR
} wifi_state_t;



/* Callback type */
typedef void (*wifi_event_cb_t)(wifi_driver_event_t event);

/* ===================== APIs ===================== */

void wifi_driver_init(void);

void wifi_driver_start(const wifi_driver_cfg_t *cfg);

void wifi_driver_stop(void);



#ifdef __cplusplus
}
#endif

#endif // WIFI_ENABLED 

#endif /* WIFI_H_ */
