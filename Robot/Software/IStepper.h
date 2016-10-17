#ifndef ISTEPPER_H
#define ISTEPPER_H

typedef unsigned char byte;

class IStepper
{
public:
	virtual void init(byte, byte, byte, byte) = 0;
	virtual void nextStep(char direction) = 0;
	virtual	void motorPinsOut(byte pins) = 0;
};

#endif // !ISTEPPER_H