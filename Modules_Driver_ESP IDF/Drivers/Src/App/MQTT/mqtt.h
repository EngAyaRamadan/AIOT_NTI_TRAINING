/*
 * mqtt.h
 *
 *  Created on: Jan 15, 2026
 *      Author: Aya Ramadan
 */

#ifndef MQTT_H_
#define MQTT_H_


#include "../../Cfg.h"


#if MQTT_ENABLED == STD_ON

#include "stdbool.h"
#include "mqtt_client.h"
#include "esp_log.h"

/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif

/* ================== TYPES ================== */
typedef enum {
    MQTT_EVT_CONNECTED,
    MQTT_EVT_DISCONNECTED,
    MQTT_EVT_DATA,
    MQTT_EVT_ERROR
} mqtt_event_t;


typedef void (*mqtt_event_cb_t)(mqtt_event_t event,const char *topic,int topic_len,const char *data,int data_len);

typedef struct {
    const char *uri;        // mqtt:// or mqtts://
    const char *client_id;
    
    mqtt_event_cb_t cb;
    
    /* Authentication (Optional) */
    const char *username;  // NULL if not used
    const char *password;  // NULL if not used

    /* TLS (Optional) */
    const char *cert_pem;  // NULL if not used
} mqtt_config_t;


/* ================== APIs ================== */
void mqtt_driver_init(const mqtt_config_t *cfg);

void mqtt_driver_publish(const char *topic,const char *payload,int qos,bool retain);

void mqtt_driver_subscribe(const char *topic, int qos);
void mqtt_driver_disconnect(void);


#ifdef __cplusplus
}
#endif


#endif /*MQTT_ENABLED*/

#endif /* MQTT_H_ */
