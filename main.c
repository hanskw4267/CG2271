#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "myLed.h"
#include "myUART.h"
#include "myMOTORS.h"


void leds()
{
	initLed();
	while(1)
	{
		runGreenLed();
		//flashRedLed(ON);
		osDelay(500);
		runGreenLed();
		//flashRedLed(OFF);
		osDelay(500);
	}
}

void movement()
{
	initUART2(BAUD_RATE);
	while(1)
	{
		uint8_t last_rx_data;
		if(rx_data != last_rx_data)
		{
			last_rx_data = rx_data;
			move(rx_data, 50);
		}
	}
}


int main(void)
{
	SystemCoreClockUpdate();
	
	osKernelInitialize();
	osThreadNew(movement, NULL, NULL);
	osThreadNew(leds, NULL, NULL);
	osKernelStart();
	return 0;
}
