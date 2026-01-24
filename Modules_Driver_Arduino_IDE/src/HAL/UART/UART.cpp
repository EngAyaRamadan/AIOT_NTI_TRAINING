#include "Arduino.h"
#include "../../App_cfg.h"
#include "UART.h"
#include <String.h>

#if UART_DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#else
#define DEBUG_PRINTLN(var)
#endif

static HardwareSerial *UART_GetHandle(UART_Port_t port)
{
    switch (port)
    {
    case UART_PORT_1:
        return &Serial1;
    case UART_PORT_2:
        return &Serial2;
    default:
        return NULL;
    }
}

void UART_Init(UART_Port_t port, uint32_t baudRate, uint32_t FrameLength, int8_t txPin, int8_t rxPin)
{
#if UART_ENABLED == STD_ON
    HardwareSerial *uart = UART_GetHandle(port);
    if (uart == NULL)
        return;

    uart->begin(baudRate, FrameLength, rxPin, txPin);
    DEBUG_PRINTLN("UART initialize");
#endif
}

void UART_SendString(UART_Port_t port, const char *str)
{
#if UART_ENABLED == STD_ON
    HardwareSerial *uart = UART_GetHandle(port);
    if (uart)
    {
        uart->print(str);
        DEBUG_PRINTLN("UART send string " + String(str));
    }
#endif
}

void UART_SendByte(UART_Port_t port, uint8_t data)
{
#if UART_ENABLED == STD_ON
    HardwareSerial *uart = UART_GetHandle(port);
    if (uart)
    {
        uart->write(data);
        DEBUG_PRINTLN("UART send byte " + String(data));
    }
#endif
}

void UART_ReadByte(UART_Port_t port, int *data)
{
#if UART_ENABLED == STD_ON
    HardwareSerial *uart = UART_GetHandle(port);
    if (!uart)
        return;

    if (uart->available())
    {
        *data = uart->read();
        DEBUG_PRINTLN("UART receive byte " + String(*data));
    }
    return;
#endif
}

String UART_ReadBuffer(UART_Port_t port)
{
#if UART_ENABLED == STD_ON
    String data = "\0";
    HardwareSerial *uart = UART_GetHandle(port);
    if (!uart)
        return "\n";

    if (uart->available())
    {
        data = uart->readStringUntil('\n');
        DEBUG_PRINTLN("UART receive data " + data);
    }

    return data;
#endif
}
