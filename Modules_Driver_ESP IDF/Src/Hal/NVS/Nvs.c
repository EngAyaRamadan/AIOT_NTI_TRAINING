/*
 * nvs.c
 *
 *  Created on: Jan 8, 2026
 *      Author: Aya Ramadan
 */

#include "Nvs.h"

#if NVS_ENABLED == STD_ON

static const char *g_Tag = TAG_NVS;


/*--------------------Initialize NVS ----------------------------------*/
esp_err_t nvs_driver_init(void) {
	
    esp_err_t ret = nvs_flash_init();
    
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // Erase and reinit
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_INIT");
    #endif
   
    return ret;
}

/*------------------------------ Write uint32 ---------------------------------*/
esp_err_t nvs_driver_set_u32(const char* namespace_name, const char* key, uint32_t value) {
	
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_u32(handle, key, value);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Set_u32 %s",key);
    #endif
    return err;
}

/*------------------------------ Read uint32 ------------------------------------*/
esp_err_t nvs_driver_get_u32(const char* namespace_name, const char* key, uint32_t* value) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    err = nvs_get_u32(handle, key, value);
    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Get_u32 %s",key);
    #endif
    return err;
}
/*------------------------------ Write string ---------------------------------*/
esp_err_t nvs_driver_set_str(const char* namespace_name,const char* key,const char* value){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_str(handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Set_string %s",key);
    #endif
    return err;
}
/*------------------------------ read string ---------------------------------*/
esp_err_t nvs_driver_get_str(const char* namespace_name,const char* key,char* value,size_t* length){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    err = nvs_get_str(handle, key, value, length);

    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Get_string %s",key);
    #endif
    return err;
}
/*------------------------------- Write blob-------------------------------------*/
esp_err_t nvs_driver_set_blob(const char* namespace_name, const char* key, const void* data, size_t length) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_blob(handle, key, data, length);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Set_Blob %s",key);
    #endif    
    return err;
}

/*--------------------------Read blob------------------------------------------------*/
esp_err_t nvs_driver_get_blob(const char* namespace_name, const char* key, void* data, size_t* length) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    err = nvs_get_blob(handle, key, data, length);
    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Get_Blob %s",key);
    #endif       
    return err;
}

/*------------------------ Erase key ------------------------------------*/
esp_err_t nvs_driver_erase_key(const char* namespace_name, const char* key) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_erase_key(handle, key);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Erase key :%s",key);
    #endif   
    return err;
}

/*---------------------------- Erase all keys in namespace ------------------------------*/
esp_err_t nvs_driver_erase_all(const char* namespace_name) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_erase_all(handle);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    #if NVS_DEBUG_ENABLED == STD_ON
      ESP_LOGI(g_Tag, "NVS_Erase name space :%s",namespace_name);
    #endif      
    return err;
}

#endif