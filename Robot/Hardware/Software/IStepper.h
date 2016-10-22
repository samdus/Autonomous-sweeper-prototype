#ifndef ISTEPPER_H
#define ISTEPPER_H

typedef unsigned char byte;

class IStepper
{
public:
	volatile virtual void init(byte, byte, byte, byte)volatile = 0;
	volatile virtual void nextStep(char direction)volatile = 0;
	volatile virtual void motorPinsOut(byte pins)volatile = 0;
};

#endif // !ISTEPPER_H