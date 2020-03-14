#ifndef MYLED_H
#define MYLED_H
	
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
	
//cycles green leds 1-by-1
void runGreenLed(void);

void flashGreenLed(state_t);


#endif
