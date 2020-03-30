#ifndef MYLED_H
#define MYLED_H

#include "stdint.h"

typedef enum {
	ON,
	OFF
}state_t;

typedef enum {
	GREEN,
	RED
}led_t;

//initialize led pins
void initLed(void);
	
//Cycles green leds 1-by-1
void runGreenLed(void);

//sets red led
void setRedLed(state_t);

// sets all the green led
void setGreenLed(state_t);

#endif
