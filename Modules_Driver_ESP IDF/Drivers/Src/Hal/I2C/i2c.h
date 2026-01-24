/*
 * i2c.h
 *
 *  Created on: Jan 13, 2026
 *      Author: Aya Ramadan
 */

#ifndef I2C_H_
#define I2C_H_

#include "../../Cfg.h"

#if I2C_ENABLED == STD_ON

#include "driver/i2c.h"
#include "esp_log.h"


/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif


/* ================= CONFIG ================= */
typedef struct
{
    i2c_port_t port;
    gpio_num_t sda_pin;
    gpio_num_t scl_pin;
    uint32_t   clk_speed;
} I2c_ConfigType;


/* ================= APIs ================= */
void I2c_Init(const I2c_ConfigType *config);

int I2c_Write(i2c_port_t port,uint8_t device_addr,const uint8_t *data,size_t size);

int I2c_Read(i2c_port_t port,uint8_t device_addr,uint8_t *data,size_t size);

int I2c_WriteRead(i2c_port_t port,uint8_t device_addr,const uint8_t *write_data,size_t write_size,uint8_t *read_data,size_t read_size);



#ifdef __cplusplus
}
#endif


#endif // I2C_ENABLED
#endif /* I2C_H_ */
