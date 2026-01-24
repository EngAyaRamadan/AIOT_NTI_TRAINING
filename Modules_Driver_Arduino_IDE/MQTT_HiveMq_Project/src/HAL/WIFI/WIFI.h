#ifndef _WIFI_H
#define _WIFI_H

#include <Arduino.h>
#include <stdint.h>

/* return codes */
typedef enum
{
    WIFI_OK = 0,
    WIFI_ERR_INVALID_ARG = -1,
    WIFI_ERR_TIMEOUT = -2,
    WIFI_ERR_FAIL = -3
} ConnectionReturn_t;

/* connection status values */
typedef enum
{
    WIFI_STATUS_DISCONNECTED = 0,
    WIFI_STATUS_CONNECTING = 1,
    WIFI_STATUS_CONNECTED = 2,
     WIFI_STATUS_ERROR = 3
} WifiStatus_t;

WifiStatus_t wifi_Init(void);
void Wifi_statemachine(void);
WifiStatus_t wifi_get_status();

#endif