#include "../../APP_cfg.h"
#include <Arduino.h>
#include <WiFi.h>
#include <cstring>
#include <cstdio>
#include "WIFI.h"

#if WIFI_DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#else
#define DEBUG_PRINTLN(var)
#endif

static WifiStatus_t gWifiState = WIFI_STATUS_DISCONNECTED;
static int gNumReconnect = 0;

// Initialize WiFi
WifiStatus_t wifi_Init(void)
{
#if WIFI_ENABLED == STD_ON

    DEBUG_PRINTLN("SSID: ");
    DEBUG_PRINTLN(WIFI_SSID);
    DEBUG_PRINTLN("Password: ");
    DEBUG_PRINTLN(WIFI_PASSWORD);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    uint64_t start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start < TIMEOUT_MS))
    {
        delay(500);
        DEBUG_PRINTLN(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        gWifiState = WIFI_STATUS_CONNECTED;
    }
    else
    {
        gWifiState = WIFI_STATUS_DISCONNECTED;
    }
    DEBUG_PRINTLN("WiFi Initialization Done");

#endif
    return gWifiState;
}

// WiFi State Machine
void Wifi_statemachine(void)
{
#if WIFI_ENABLED == STD_ON

    switch (gWifiState)
    {
    case WIFI_STATUS_CONNECTED:
        if (WiFi.status() != WL_CONNECTED)
        {
            gWifiState = WIFI_STATUS_DISCONNECTED;
            DEBUG_PRINTLN("[WiFi] State: Disconnected");
            gNumReconnect = 0;
        }
        else
        {
            DEBUG_PRINTLN("[WiFi] State: Connected!");
        }
        break;

    case WIFI_STATUS_DISCONNECTED:
    {
        DEBUG_PRINTLN("[WiFi] Attempting to reconnect...");
        gWifiState = WIFI_STATUS_CONNECTING;
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        gNumReconnect = 0;
    }
    break;

    case WIFI_STATUS_CONNECTING:
        if (WiFi.status() == WL_CONNECTED)
        {
            gWifiState = WIFI_STATUS_CONNECTED;
            DEBUG_PRINTLN("[WiFi] Connected successfully!");
        }
        else
        {
            gNumReconnect++;
            DEBUG_PRINTLN("[WiFi] Connecting...");
            if (gNumReconnect >= WIFI_RECONNECT_TIMEOUT)
            {
                gWifiState = WIFI_STATUS_ERROR;
                DEBUG_PRINTLN("[WiFi] Connection Error!");
            }
        }
        break;

    case WIFI_STATUS_ERROR:
    {
        DEBUG_PRINTLN("[WiFi] Error state! Resetting WiFi...");
        WiFi.disconnect();
        gWifiState = WIFI_STATUS_DISCONNECTED;
    }
    break;

    default:
        gWifiState = WIFI_STATUS_ERROR;
        break;
    }

#endif
}

// Get current WiFi status
WifiStatus_t wifi_get_status()
{
#if WIFI_ENABLED == STD_ON
    return gWifiState;
#endif
}
