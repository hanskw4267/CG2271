#include "MKL25Z4.h"                    // Device header
#include "myUART.h"

volatile uint8_t tx_data;
volatile uint8_t rx_data;
void UART2_IRQHandler()
{
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//receive when reg is full
	if (UART2->S1 & UART_S1_RDRF_MASK)
	{
		rx_data = (UART2->D);
	}
	
	//error check and handler
	if ((UART2->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)))
	{
	}
}


void initUART2(uint32_t baud_rate)
{
	uint32_t divisor, bus_clock;
	
	//enable clock on portD
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	//clear and set uart rx port as mux mode 4
	PORTD->PCR[UART_RX] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[UART_RX] |= PORT_PCR_MUX(3);
	
	//disable TX/RX
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	//sets params for the uart
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	//enable RX
	UART2->C2 |= ((UART_C2_RE_MASK));
	
	//enable interrupt
	NVIC_SetPriority(UART2_IRQn, 1);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	//enable TX/RX interrupts
	UART2->C2 |= UART_C2_RIE_MASK;
}
