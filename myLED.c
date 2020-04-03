//TODO: Write red led code

#include "MKL25Z4.h"                    // Device header
#include <stdbool.h>
#include "myLED.h"
#include "cmsis_os2.h" 
#include <time.h>
#define MASK(x) (1 << (x))

//Port C pin arrays to store green led pin number
int greenpins[] = {7, 0, 3, 4, 5, 6, 10, 11};

// Port A pin to store red led pin number
int redpins[] = {1,2};

//counter used to cycle green
int counter = 0;


void initLed()
{
	//enable clock to portC and portA
	SIM->SCGC5 |= ((SIM_SCGC5_PORTC_MASK));
	SIM ->SCGC5 |= ((SIM_SCGC5_PORTA_MASK));
	
	for (int i = 0; i < sizeof(greenpins)/sizeof(int); i++)
	{
	  //configure MUX settings to make the Green LED pins GPIO
		PORTC->PCR[greenpins[i]] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[greenpins[i]] |= PORT_PCR_MUX(1);
		
		//Set direction to output
	  PTC->PDDR |= MASK(greenpins[i]);
	}
	
	// Configure red led settings
	for (int i = 0; i < sizeof(redpins)/sizeof(int); i += 1){
			PORTA ->PCR[redpins[i]] &= ~PORT_PCR_MUX_MASK;
		  PORTA ->PCR[redpins[i]] |= PORT_PCR_MUX(1);
		  PTA->PDDR |= MASK(redpins[i]);
		
	}
}

void runGreenLed()
{
	PTC->PDOR = MASK(greenpins[counter]);
	counter++;
	if (counter > 7)
	{
		counter = 0;
	}
}

void setRedLed(state_t state)
{
	for (int i = 0; i < sizeof(redpins)/sizeof(int); i++)
	{
		if(state == ON)
		{
			PTA->PDOR |= MASK(redpins[i]);
		}
		else
		{
			PTA->PDOR &= ~MASK(redpins[i]);
		}
	}
}

void setGreenLed(state_t state){
	
	if (state == ON)
	{
	  for (int i = 0; i < sizeof(greenpins)/sizeof(int); i++)
		{
				PTC -> PDOR |= MASK(greenpins[i]);
		}
	}
	else
	{
		for (int i = 0; i < sizeof(greenpins)/sizeof(int); i++)
		{
				PTC -> PDOR &= ~MASK(greenpins[i]);
		}
	}
}
