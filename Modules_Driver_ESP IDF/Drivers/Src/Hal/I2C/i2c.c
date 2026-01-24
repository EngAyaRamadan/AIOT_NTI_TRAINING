/*
 * i2c.c
 *
 *  Created on: Jan 13, 2026
 *      Author: Aya Ramadan
 */

#include "i2c.h"

#if I2C_ENABLED == STD_ON

static const char *g_Tag = TAG_I2C;

/* Track initialized ports */
static uint8_t g_I2cInitialized[I2C_NUM_MAX] = {0};

/* ================= INIT ================= */
void I2c_Init(const I2c_ConfigType *config)
{
    if (!config || config->port >= I2C_NUM_MAX)
        return ;

    if (g_I2cInitialized[config->port])
        return ;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = config->sda_pin,
        .scl_io_num = config->scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = config->clk_speed
    };

    // Configure I2C parameters
    i2c_param_config(config->port, &conf) ;
        
       // Install I2C driver
    i2c_driver_install(config->port, conf.mode, 0, 0, 0); 
       

    g_I2cInitialized[config->port] = 1;

#if I2C_DEBUG_ENABLED
    ESP_LOGI(g_Tag,
             "I2C Init | Port=%d SDA=%d SCL=%d CLK=%lu",
             config->port,config->sda_pin,config->scl_pin,config->clk_speed);
#endif

}

/* ================= WRITE ================= */
int I2c_Write(i2c_port_t port,uint8_t device_addr,const uint8_t *data,size_t size)
{
   // Create I2C command link
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    // Start bit
    i2c_master_start(cmd);
    // Write device address + write bit
    i2c_master_write_byte(cmd,(device_addr << 1) | I2C_MASTER_WRITE, true);
     // Write data
    i2c_master_write(cmd, data, size, true);
    // Stop bit
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin( port, cmd, pdMS_TO_TICKS(1000));
    // Delete command link
    i2c_cmd_link_delete(cmd);

#if I2C_DEBUG_ENABLED == STD_ON
    if (ret == ESP_OK) {
        ESP_LOGI(g_Tag, "Read from 0x%02X: %d bytes", device_addr, size);
    } else {
        ESP_LOGE(g_Tag, "Read from 0x%02X failed: %s", device_addr, esp_err_to_name(ret));
    }
#endif

    return (ret == ESP_OK) ? 0 : -1;
}

/* ================= READ ================= */
int I2c_Read(i2c_port_t port,uint8_t device_addr,uint8_t *data,size_t size)
{
       // Create I2C command link
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
     // Start bit
    i2c_master_start(cmd);
     // Write device address + read bit
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_READ, true);
   
   // Read data
    if (size > 1)
        i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);

    i2c_master_read_byte(cmd,
        data + size - 1, I2C_MASTER_NACK);
    // Stop bit
    i2c_master_stop(cmd);

    // Execute command
    esp_err_t ret = i2c_master_cmd_begin(
        port, cmd, pdMS_TO_TICKS(1000));
  
    // Delete command link
    i2c_cmd_link_delete(cmd);

#if I2C_DEBUG_ENABLED == STD_ON
    if (ret == ESP_OK) {
        ESP_LOGI(g_Tag, "Read from 0x%02X: %d bytes", device_addr, size);
    } else {
        ESP_LOGE(g_Tag, "Read from 0x%02X failed: %s", device_addr, esp_err_to_name(ret));
    }
#endif

    return (ret == ESP_OK) ? 0 : -1;
}

/* ============== WRITE + READ ============== */
int I2c_WriteRead(i2c_port_t port,uint8_t device_addr,const uint8_t *write_data,size_t write_size,uint8_t *read_data,size_t read_size)
{
    // Create I2C command link
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

     // Start bit
    i2c_master_start(cmd);
     // Write device address + write bit
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, true);
     // Write data (usually register address)
    i2c_master_write(cmd, write_data, write_size, true);
  
    // Repeated start
    i2c_master_start(cmd);

    // Write device address + read bit
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_READ, true);

    // Read data
    if (read_size > 1)
        i2c_master_read(cmd, read_data, read_size - 1, I2C_MASTER_ACK);

    i2c_master_read_byte(cmd, read_data + read_size - 1, I2C_MASTER_NACK);

    // Stop bit
    i2c_master_stop(cmd);

    // Execute command
    esp_err_t ret = i2c_master_cmd_begin( port, cmd, pdMS_TO_TICKS(1000));

    // Delete command link
    i2c_cmd_link_delete(cmd);

#if I2C_DEBUG_ENABLED == STD_ON
    if (ret == ESP_OK) {
        ESP_LOGI(g_Tag, "WriteRead 0x%02X: W=%d, R=%d bytes", device_addr, write_size, read_size);
    } else {
        ESP_LOGE(g_Tag, "WriteRead 0x%02X failed: %s", device_addr, esp_err_to_name(ret));
    }
#endif

    return (ret == ESP_OK) ? 0 : -1;
}

#endif