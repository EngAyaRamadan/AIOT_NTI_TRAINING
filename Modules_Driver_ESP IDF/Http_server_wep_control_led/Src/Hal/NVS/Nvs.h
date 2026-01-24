#ifndef NVS_DRIVER_H
#define NVS_DRIVER_H

#include "../../Cfg.h"

#if NVS_ENABLED == STD_ON


#include <stdint.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"


#ifdef __cplusplus
extern "C" {
#endif

// Init NVS
esp_err_t nvs_driver_init(void);
// Write uint32
esp_err_t nvs_driver_set_u32(const char* namespace_name, const char* key, uint32_t value);
// Read uint32
esp_err_t nvs_driver_get_u32(const char* namespace_name, const char* key, uint32_t* value);

// Write string
esp_err_t nvs_driver_set_str(const char* namespace_name,const char* key,const char* value);
// Read string
// value: buffer provided by user
// length: input = buffer size, output = actual string length
esp_err_t nvs_driver_get_str(const char* namespace_name,const char* key,char* value,size_t* length);

// Write blob
esp_err_t nvs_driver_set_blob(const char* namespace_name, const char* key, const void* data, size_t length);
// Read blob
esp_err_t nvs_driver_get_blob(const char* namespace_name, const char* key, void* data, size_t* length);

// Erase key
esp_err_t nvs_driver_erase_key(const char* namespace_name, const char* key);
// Erase all keys in namespace
esp_err_t nvs_driver_erase_all(const char* namespace_name);



#ifdef __cplusplus
}
#endif


#endif // NVS_ENABLED
#endif /* NVS_H_ */