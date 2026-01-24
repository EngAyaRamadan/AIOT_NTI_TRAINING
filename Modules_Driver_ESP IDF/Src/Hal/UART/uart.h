/*
 * uart.h
 *
 *  Created on: Jan 13, 2026
 *      Author: Aya Ramadan
 */
 
 /*
 * Driver support -> polling
                  -> interrupted based using Queue
 *
 */

#ifndef UART_H_
#define UART_H_

#include "../../Cfg.h"


#if UART_ENABLED == STD_ON

#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "esp_log.h"


/* --------------------------------------------------
* developed using both C and C++ 
* C function to be callable from C++
* it has to be declared with C linkage (extern "C")
*/
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uart_port_t uart_num;
    int baud_rate;
    gpio_num_t tx_pin;
    gpio_num_t rx_pin;
} Uart_ConfigType;

/* APIs */
void Uart_Init(const Uart_ConfigType *cfg);
QueueHandle_t Uart_GetEventQueue(uart_port_t uart_num);
int Uart_Write(uart_port_t uart_num, const uint8_t *data, uint16_t len);
int Uart_Read(uart_port_t uart_num, uint8_t *data, uint16_t len,uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif


#endif //UART_ENABLED

#endif /* UART_H_ */
