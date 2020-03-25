#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "myLED.h"
#include "myUART.h"
#include "myMOTORS.h"


void leds(void *argument)
{
	  while(1){
			if(rx_data == 1){
				//runGreenLed();
			  flashRedLed(ON);
			  osDelay(500);
			  flashRedLed(OFF);
			  osDelay(500);
			}
			if (rx_data == 0){
			  //lightGreenLed();
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
		/**uint8_t last_rx_data;
		if(rx_data != last_rx_data)
		{
			last_rx_data = rx_data;
			move(rx_data, 50);
		}**/
		move(FORWARD,30);
	}
}

void buzzer(void *argument){

}

void brain(void *argument){

}


int main(void)
{
	SystemCoreClockUpdate();
	//initMotors();
	initUART2(BAUD_RATE);
	initLed();
	
	osKernelInitialize();
	osThreadNew(leds, NULL, NULL);
	//osThreadNew(movement,NULL,NULL);
	osKernelStart();
	return 0;
}
