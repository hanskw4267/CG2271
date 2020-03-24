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

//Flash red led
void flashRedLed(state_t);

// Lights up all the green led
void lightGreenLed(void);

// Flash 2 green led twice when bluetooth connected
void flashGreenLedTwice(void);

void delay(volatile uint32_t nof);

#endif
