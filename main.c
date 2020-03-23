#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "myLed.h"
#include "myUART.h"
#include "myMOTORS.h"


void leds(void *argument)
{
	 
	  while(1){
			if(rx_data == 3){
				runGreenLed();
			  flashRedLed(ON);
			  osDelay(500);
			  flashRedLed(OFF);
			  osDelay(500);
			}
			if (rx_data == 2){
			  lightGreenLed();
			  flashRedLed(ON);
			  osDelay(250);
			  flashRedLed(OFF);
			  osDelay(250);
			}
		}
	
}

void movement(void *argument)
{
	
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
	initLed();
	initUART2(BAUD_RATE);
	
	osKernelInitialize();
	osThreadNew(leds, NULL, NULL);
	osKernelStart();
	return 0;
}
