#ifndef ISTEPPER_H
#define ISTEPPER_H

typedef unsigned char byte;

#define STEPPER_NB_MOTEUR 2 
#define STEPPER_GAUCHE 0
#define STEPPER_DROIT 1

class IStepper
{
public:
	 virtual void init(byte, byte, byte, byte) = 0;
	 virtual void nextStep(char direction) = 0;
	 virtual void motorPinsOut(byte pins) = 0;
};

#endif // !ISTEPPER_H