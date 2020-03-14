//TODO: Write red led code

#include "MKL25Z4.h"                    // Device header
#include <stdbool.h>
#include "myLED.h"
#define MASK(x) (1 << (x))

//arrays to store led pin number
int greenpins[] = {7, 0, 3, 4, 5, 6, 10, 11};
int redpins[] = {};

//counter used to cycle green
int counter = 0;

void initLed()
{
	//enable clock to portC
	SIM->SCGC5 |= ((SIM_SCGC5_PORTC_MASK));
	for (int i = 0; i < sizeof(greenpins)/sizeof(int); i++)
	{
	//configure MUX settings to make the Green LED pins GPIO
	PORTC->PCR[greenpins[i]] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[greenpins[i]] |= PORT_PCR_MUX(1);
	
	//Set direction to output
	PTC->PDDR |= MASK(greenpins[i]);
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

void flashGreenLed(state_t state)
{
	for (int i = 0; i < sizeof(greenpins)/sizeof(int); i++)
	{
		if(state == ON)
		{
			PTC->PDOR |= MASK(greenpins[i]);
		}
		else
		{
			PTC->PDOR &= ~MASK(greenpins[i]);
		}
	}
}
