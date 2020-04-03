#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "myLED.h"
#include "myUART.h"
#include "myMOTORS.h"
#include "myBuzzer.h"

osSemaphoreId_t startSem, movingSem;
osMessageQueueId_t pendingCmds;
osEventFlagsId_t led, music;
osThreadId_t tid_vader;

const osThreadAttr_t highThread_attr = 
{
	.priority = osPriorityAboveNormal
};

const osThreadAttr_t lowThread_attr = 
{
	.priority = osPriorityBelowNormal
};

void tStart(void *argument)
{
	while(1)
	{
		osSemaphoreAcquire(startSem,osWaitForever);
		setGreenLed(ON);
		osDelay(500);
		setGreenLed(OFF);
		osDelay(500);
		setGreenLed(ON);
		osDelay(500);
		setGreenLed(OFF);
	}
}

void tMotors(void *argument)
{
	cmdPkt cmd;
	while(1)
	{
		osMessageQueueGet(pendingCmds, &cmd, NULL, osWaitForever);
		move(cmd.direction, 100.0);
	}
}

void tRunLed(void *argument)
{
	while(1)
	{
		osEventFlagsWait(led, 0x01,osFlagsNoClear, osWaitForever);
		runGreenLed();
		setRedLed(ON);
		osDelay(500);
		setRedLed(OFF);
		osDelay(500);
	}
}

void tFlashLed(void *argument)
{
	while(1)
	{
		osEventFlagsWait(led, 0x10,osFlagsNoClear, osWaitForever);
		setGreenLed(ON);
		setRedLed(ON);
		osDelay(250);
		setRedLed(OFF);
		osDelay(250);
	}
}

void tEnd(void *argument)
{
	while(1)
	{
		osEventFlagsWait(music, 0x10 ,NULL, osWaitForever);
		playEndTone();
	}
}

void tSong(void *argument)
{
	while(1)
	{
		osEventFlagsWait(music, 0x01,osFlagsNoClear, osWaitForever);
		playSong();
	}
}



void tBrain(void *argument){
	cmdPkt newCmd;
	while(1)
	{
		osThreadFlagsWait(0x0001 , NULL, osWaitForever);
		newCmd.direction = rx_data;
		osMessageQueuePut(pendingCmds, &newCmd, NULL, 2000);
		if(newCmd.direction == 9)
		{
			osSemaphoreRelease(startSem);
			osThreadResume(tid_vader);
			osEventFlagsSet(music, 0x01);
		}
		else if (newCmd.direction == 10)
		{
			osThreadSuspend(tid_vader);
			osEventFlagsClear(music,0x01);
			osEventFlagsSet(music, 0x10);
		}
		else if (newCmd.direction == 0)
		{
			osEventFlagsClear(led,0x01);
			osEventFlagsSet(led,0x10);
		}
		else
		{
			osEventFlagsClear(led,0x10);
			osEventFlagsSet(led,0x01);
		}
			
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
	
	pendingCmds = osMessageQueueNew(1,sizeof(cmdPkt),NULL);
	
	led = osEventFlagsNew(NULL);
	music = osEventFlagsNew(NULL);
	
	startSem = osSemaphoreNew(1, 0, NULL);
	movingSem = osSemaphoreNew(1, 0, NULL);
	
	tid_tBrain = osThreadNew(tBrain,NULL,&highThread_attr);
	osThreadNew(tStart,NULL,NULL);
	osThreadNew(tMotors,NULL,NULL);
	osThreadNew(tEnd,NULL,NULL);
	osThreadNew(tRunLed, NULL,NULL);
	osThreadNew(tFlashLed, NULL,NULL);
	tid_vader = osThreadNew(tSong,NULL,&lowThread_attr);
	
	osKernelStart();
	for(;;){}
}
