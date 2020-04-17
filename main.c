#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "myLED.h"
#include "myUART.h"
#include "myMOTORS.h"
#include "myBuzzer.h"

osMessageQueueId_t pendingCmds;
osEventFlagsId_t led, music;
osThreadId_t tid_vader, tid_tBrain;

const osThreadAttr_t highThread_attr = 
{
	.priority = osPriorityAboveNormal
};

const osThreadAttr_t lowThread_attr = 
{
	.priority = osPriorityBelowNormal
};



void tMotors(void *argument)
{
	cmdPkt cmd;
	while(1)
	{
		osMessageQueueGet(pendingCmds, &cmd, NULL, osWaitForever);
		move(cmd.direction, 100.0);
	}
}


/**
* LED threads to control led states
* tStartLeds() runs once when BT connects
* tRunLed() runs when the robot is moving
* tFlashLed() runs when the robot is stationary
*/
void tStartLeds(void *argument)
{
	while(1)
	{
		osEventFlagsWait(led, 0x100, NULL, osWaitForever);
		setGreenLed(ON);
		osDelay(500);
		setGreenLed(OFF);
		osDelay(500);
		setGreenLed(ON);
		osDelay(500);
		setGreenLed(OFF);
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
		setGreenLed(OFF);
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
		setGreenLed(OFF);
	}
}

/**
* LED threads to control led states
* tStartSong() runs once when BT connects
* tEndSong() runs once when BT disconnects
* tSong() runs constantly
*/
void tStartSong(void *argument)
{
	while(1)
	{
		osEventFlagsWait(music, 0x100 ,NULL, osWaitForever);
		playStart();
		osThreadResume(tid_vader);
		osEventFlagsSet(music, 0x001);
	}
}

void tEndSong(void *argument)
{
	while(1)
	{
		osEventFlagsWait(music, 0x010 ,NULL, osWaitForever);
		playEndTone();
	}
}

void tSong(void *argument)
{
	while(1)
	{
		osEventFlagsWait(music, 0x001,osFlagsNoClear, osWaitForever);
		playSong();
	}
}



void tBrain(void *argument){
	cmdPkt newCmd;
	while(1)
	{
		osThreadFlagsWait(0x1 , NULL, osWaitForever);
		newCmd.direction = rx_data;
		osMessageQueuePut(pendingCmds, &newCmd, NULL, 2000);
		if(newCmd.direction == START)
		{
			osEventFlagsSet(led, 0x100);
			osEventFlagsSet(music, 0x100);
		}
		else if (newCmd.direction == END)
		{
			osThreadSuspend(tid_vader);
			osEventFlagsClear(music,0x001);
			osEventFlagsSet(music, 0x010);
			osEventFlagsClear(led,0x11);
		}
		else if (newCmd.direction == STOP)
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
	
	tid_tBrain = osThreadNew(tBrain,NULL,&highThread_attr);
	osThreadNew(tStartLeds,NULL,NULL);
	osThreadNew(tMotors,NULL,NULL);
	osThreadNew(tEndSong,NULL,NULL);
	osThreadNew(tStartSong,NULL,NULL);
	osThreadNew(tRunLed, NULL,NULL);
	osThreadNew(tFlashLed, NULL,NULL);
	tid_vader = osThreadNew(tSong,NULL,&lowThread_attr);
	
	osKernelStart();
	for(;;){}
}
