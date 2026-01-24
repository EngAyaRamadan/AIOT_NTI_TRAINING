/*
 * uart.c
 *
 *  Created on: Jan 13, 2026
 *      Author: Aya Ramadan
 */

#include "uart.h"

#if UART_ENABLED == STD_ON

static const char *g_Tag = TAG_UART;

/* Event queue per UART */
static QueueHandle_t uart_event_queue[UART_NUM_MAX] = {NULL};

void Uart_Init(const Uart_ConfigType *cfg)
{
    uart_config_t uart_config =
    {
        .baud_rate = cfg->baud_rate,
        .data_bits = UART_FRAME_LENGTH,
        .parity    = UART_PARITY,
        .stop_bits = UART_STOP_BIT,
        .flow_ctrl = UART_CTRL,
        .source_clk = UART_SCLK_APB
    };

    uart_param_config(cfg->uart_num, &uart_config);

    uart_set_pin(cfg->uart_num,
                 cfg->tx_pin,
                 cfg->rx_pin,
                 UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE);

    uart_driver_install(cfg->uart_num,
                        1024,   // RX buffer
                        0,      // TX buffer
                        10,     // Event queue size
                        &uart_event_queue[cfg->uart_num],
                        0);
                        
 #if UART_DEBUG_ENABLED == STD_ON 
    ESP_LOGI(g_Tag, "UART%d initialized with Queue", cfg->uart_num);
    ESP_LOGI(g_Tag, "Port: %d, TX: %d, RX: %d, Baud: %d",
             cfg->uart_num,cfg->tx_pin,cfg->rx_pin,cfg->baud_rate);
 #endif   
}

QueueHandle_t Uart_GetEventQueue(uart_port_t uart_num)
{
	 #if UART_DEBUG_ENABLED == STD_ON 
      ESP_LOGI(g_Tag, "return UART%d Queue for interrupt based ", uart_num);
     #endif   
    return uart_event_queue[uart_num];
}


int Uart_Write(uart_port_t uart_num, const uint8_t *data, uint16_t len)
{
    int bytes_written = uart_write_bytes(uart_num, (const char *)data, len);
    
    #if UART_DEBUG_ENABLED == STD_ON
        if (bytes_written > 0)
            ESP_LOGI(g_Tag, "UART%d Wrote %d bytes", uart_num, bytes_written);
        else
            ESP_LOGE(g_Tag, "UART%d Write failed", uart_num);
    #endif

    return bytes_written;  
}


int Uart_Read(uart_port_t uart_num, uint8_t *data, uint16_t len,uint32_t timeout_ms)
{
	int bytes_read = uart_read_bytes(uart_num, data, len, timeout_ms);
	
	#if UART_DEBUG_ENABLED == STD_ON
	   if (bytes_read > 0)
           ESP_LOGI(g_Tag, "UART%d Read %d bytes ", uart_num, bytes_read);
    #endif   
   
    return bytes_read;
}


#endif