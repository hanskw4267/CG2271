#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "myLED.h"
#include "myUART.h"
#include "myMOTORS.h"
#include "myBuzzer.h"

osSemaphoreId_t motorSem, ledSem;

void tLED(void *argument)
{
	  while(1){
			if (rx_data == 9)
			{
				setGreenLed(OFF);
				osDelay(500);
				setGreenLed(ON);
				osDelay(500);
				setGreenLed(OFF);
				osDelay(500);
				setGreenLed(ON);
				rx_data = 0;
			}
			else if (rx_data != 0 ){
				runGreenLed();
			  setRedLed(ON);
			  osDelay(500);
			  setRedLed(OFF);
			  osDelay(500);
			}
			else{
				setGreenLed(ON);
			  setRedLed(ON);
			  osDelay(250);
			  setRedLed(OFF);
			  osDelay(250);
			}
			 
		}
}

void tMotorControl(void *argument)
{
	while(1)
	{
		osSemaphoreAcquire(motorSem, osWaitForever);
		 move(rx_data, 100.0);
	}
	
}

void tAudio(void *argument){
	while(1){
		playSong();
	}
}

void tBrain(void *argument){
	
	
	while(1){
		 osEventFlagsWait(newData, 0x1u, NULL, osWaitForever);
		 osSemaphoreRelease(motorSem);
		 osSemaphoreRelease(ledSem);
	}
}


int main(void)
{
	SystemCoreClockUpdate();
	initMotors();
	initUART2(BAUD_RATE);
	initBuzzer();
	initLed();
	
	
	osKernelInitialize();
	//motorSem = osSemaphoreNew(1, 0, NULL);
	//ledSem = osSemaphoreNew(1, 0, NULL);
	
	osThreadNew(tBrain,NULL,NULL);
	osThreadNew(tAudio,NULL,NULL);
	osThreadNew(tLED, NULL, NULL);
	osThreadNew(tMotorControl,NULL,NULL);
	osKernelStart();
	for(;;){}
}
