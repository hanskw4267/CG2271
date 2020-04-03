#ifndef MYUART_H
#define MYUART_H
#include "stdint.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2

extern volatile uint8_t rx_data;
extern osThreadId_t tid_tBrain;

typedef enum
{
	UART_TX = 3,
	UART_RX = 2,
	UART2_INT_PRIO = 128,
	BAUD_RATE = 9600
}uart_config_t;

void initUART2(uint32_t);

#endif
