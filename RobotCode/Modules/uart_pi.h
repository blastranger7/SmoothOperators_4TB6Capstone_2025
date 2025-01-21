#ifndef UART_PI_H
#define UART_PI_H

#include "stm32f4xx_hal_uart.h"

typedef struct {
    UART_HandleTypeDef* port;
    uint8_t rx_buffer[3];
    int updated;
} uart_port; 

uart_port* initUART(UART_HandleTypeDef* port);
void sendData(uart_port* uart, uint8_t data);
void getData(uart_port* uart, uint8_t* data);
int isDataUpdated(uart_port* uart);

#endif