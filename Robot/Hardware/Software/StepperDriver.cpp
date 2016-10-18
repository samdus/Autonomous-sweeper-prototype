#include "StepperDriver.h"

StepperDriver::StepperDriver(IStepper* stepper)
{
	_stepper = stepper;
	_compteur = 1;
	_immobile = true;
	_direction = AVANT;
	_vitesse = VITESSES[8];
}

StepperDriver::~StepperDriver(){}

void StepperDriver::step()
{
	if (!_immobile)
	{
		if ((_vitesse & _compteur) > 0)
			_stepper->nextStep(*_direction);

		if ((_compteur <<= 1) == 0)
			_compteur = 1;
	}
}

void StepperDriver::setVitesse(unsigned short vitesse)
{
	if (vitesse > 8)
		vitesse = 8;
	_vitesse = VITESSES[vitesse];
}

void StepperDriver::avant()
{
	_direction = AVANT;
}

void StepperDriver::derriere()
{
	_direction = DERRIERE;
}

void StepperDriver::gauche()
{
	_direction = GAUCHE;
}

void StepperDriver::droite()
{
	_direction = DROITE;
}

void StepperDriver::avance()
{
	_immobile = false;
}

void StepperDriver::stop()
{
	_immobile = true;
}

const char StepperDriver::getDirection()
{
	if (_direction[0] > 0 && _direction[1] > 0)
		return 'R';
	
	if (_direction[0] > 0 && _direction[1] < 0)
		return 'G';

	if (_direction[0] < 0 && _direction[1] > 0)
		return 'D';

	if (_direction[0] < 0 && _direction[1] < 0)
		return 'A';
}

const unsigned short StepperDriver::getVitesse()
{
	for (unsigned short i = 0; i < 7; ++i)
		if (_vitesse == VITESSES[i])
			return i;
	return 8;
}

bool StepperDriver::getImmobile()
{
	return _immobile;
}
