#ifndef UART_MODULE_H
#define UART_MODULE_H

#include <stdint.h>

typedef enum
{
    UART_PORT_1 = 1,
    UART_PORT_2 = 2
} UART_Port_t;

typedef struct
{
    UART_Port_t port;
    uint32_t baudRate;
    uint32_t FrameLength;
    int8_t txPin;
    int8_t rxPin;
} UART_Config_t;

void UART_Init(UART_Port_t port, uint32_t baudRate, uint32_t FrameLength, int8_t txPin, int8_t rxPin);
void UART_SendString(UART_Port_t port, const char *str);
void UART_SendByte(UART_Port_t port, uint8_t data);

void UART_ReadByte(UART_Port_t port, int *data);
String UART_ReadBuffer(UART_Port_t port);

#endif
