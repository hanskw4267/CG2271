#ifndef MYUART_H
#define MYUART_H
#include "stdint.h"

extern volatile uint8_t tx_data;
extern volatile uint8_t rx_data;

typedef enum
{
	UART_TX = 3,
	UART_RX = 2,
	UART2_INT_PRIO = 128,
	BAUD_RATE = 9600
}uart_config_t;

void initUART2(uint32_t);

#endif
