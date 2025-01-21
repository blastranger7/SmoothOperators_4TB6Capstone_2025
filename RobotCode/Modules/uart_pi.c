#include "uart_pi.h"

uart_port* initUART(UART_HandleTypeDef* port) {
    uart_port* uart;
    
    uart->port = port;
    uart->rx_buffer = {};
    uart->updated = 0;

    return uart;
}

void sendData(uart_port* uart, uint8_t data) {
    HAL_UART_Transmit_IT(&uart, &data, 1);
}

void getData(uart_port* uart, uint8_t* data) {
    for (int i = 0; i < 2; i++) {
        *(data + i) = uart->rx_buffer[i];
    }
}

int isDataUpdated(uart_port* uart) {
    return uart->updated; 
}