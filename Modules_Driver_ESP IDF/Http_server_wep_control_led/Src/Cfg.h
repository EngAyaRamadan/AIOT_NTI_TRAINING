/*
 * Cfg.h
 *
 *  Created on: Jan 8, 2026
 *      Author:Aya Ramadan
 */

#ifndef CFG_H_
#define CFG_H_


//==============================Standard definitions ========================================
#define STD_ON      1 // Enabled
#define STD_OFF     0 // Disabled

//====================================	TAGS ================================================
//------------------Tags for Hal logging------------//
#define TAG_GPIO "GPIO_DRIVER"
#define TAG_NVS  "NVS_DRIVER"
#define TAG_WIFI "WIFI_DRIVER"

//------------------Tags for App logging------------//
#define TAG_HTTP_SERVER "HTTP_SERVER_DRIVER"

//====================================	Enable or disable ===================================
//------------------Enable or disable Hal drivers------------//
#define GPIO_ENABLED            STD_ON
#define NVS_ENABLED             STD_ON
#define WIFI_ENABLED            STD_ON


//------------------Enable or disable App drivers------------//

#define HTTP_SERVER_ENABLED     STD_ON

//=============================Enable or disable debug ======================================
//------------------Enable or disable debug logs for Hal drivers------------//
// GPIO Debugging
#if GPIO_ENABLED == STD_ON
#define GPIO_DEBUG_ENABLED      STD_OFF
#endif

//NVS Debugging
#if NVS_ENABLED == STD_ON
#define NVS_DEBUG_ENABLED       STD_ON
#endif

//WIFI Debugging
#if WIFI_ENABLED == STD_ON
#define WIFI_DEBUG_ENABLED      STD_ON
#endif


//------------------Enable or disable debug logs for App drivers------------//

//HTTP server Debugging
#if HTTP_SERVER_ENABLED  == STD_ON
#define HTTP_SERVER_DEBUG_ENABLED  STD_ON
#endif


//======================================Config ===========================================

//------------- Hal config ------------------------------------------


//------------- Wifi config --------------------------
#define WIFI_MAX_RETRY 5


/*--------Hard Cred-------------*/

#define WIFI_STA_SSID    "Aya Ramadan"
#define WIFI_STA_PASS    "ayaramadan22"

#define WIFI_AP_SSID     "ESP32_Config"
#define WIFI_AP_PASS     "ayaramadan"


/*--------NVS Cred-------------*/
#define WIFI_NVS_NAMESPACE   "wifi"

#define WIFI_NVS_STA_SSID    "sta_ssid"
#define WIFI_NVS_STA_PASS    "sta_pass"

#define WIFI_NVS_AP_SSID     "ap_ssid"
#define WIFI_NVS_AP_PASS     "ap_pass"


//------------- App config ------------------------------------------

//------------- Http Server Config ---------------------------
/* for HTTPS */
#define HTTP_SERVER_USE_SSL    0
/*config server*/
#define HTTP_SERVER_PORT      80 /*https-> 443 , http-> 80*/               
     


#endif /* CFG_H_ */
