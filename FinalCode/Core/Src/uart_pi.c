#include "uart_pi.h"

void initUART(UART_HandleTypeDef* port, uart_port* u_port) {
    u_port->port = port;
    u_port->rx_buffer[0] = 0;
    u_port->rx_buffer[1] = 0;
    u_port->rx_buffer[2] = 0;
    u_port->updated = 0;
}

void sendData(uart_port* uart, uint8_t data) {
    HAL_UART_Transmit_IT(uart->port, &data, 1);
}

void getData(uart_port* uart, int* data) {
    data[0] = (int) ((uart->rx_buffer[1] << 8) | uart->rx_buffer[0]);
		data[1] = (int) ((uart->rx_buffer[3] << 8) | uart->rx_buffer[2]);
		data[2] = (int) ((uart->rx_buffer[5] << 8) | uart->rx_buffer[4]);
		uart->updated = 0;
}

int isDataUpdated(uart_port* uart) {
    return uart->updated; 
}