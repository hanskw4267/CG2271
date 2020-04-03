#ifndef MYMOTORS_H
#define MYMOTORS_H

typedef enum
{
	FORWARD = 1,
	REVERSE = 2,
	RIGHT = 3,
	LEFT = 4,
	FORWARDRIGHT = 5,
	FORWARDLEFT = 6,
	REVERSERIGHT = 7,
	REVERSELEFT = 8,
	STOP = 0
}direction_t;

typedef struct
{
	direction_t direction;
}cmdPkt;

void initMotors(void);

void move(direction_t, float);
#endif
