#include "StepperDriver.h"

StepperDriver::StepperDriver(volatile IStepper* stepper, unsigned short noMoteur)
{
	_noMoteur = noMoteur;
	_stepper = stepper;
	_compteur = 1;
	_enMouvement = false;
	_direction = AVANT[_noMoteur];
	_vitesse = VITESSES[8];
}

StepperDriver::~StepperDriver(){}

volatile void StepperDriver::init(byte mPin_1, byte mPin_2, byte mPin_3, byte mPin_4) volatile
{
	_stepper->init(mPin_1, mPin_2, mPin_3, mPin_4);
}

volatile void StepperDriver::step()volatile
{
	if (_enMouvement)
	{
		if ((_vitesse & _compteur) > 0)
			_stepper->nextStep(_direction);

		if ((_compteur <<= 1) == 0)
			_compteur = 1;
	}
}

volatile void StepperDriver::setVitesse(unsigned short vitesse)volatile
{
    if (vitesse > 8)
        vitesse = 8;

	_vitesse = VITESSES[vitesse];
}

volatile void StepperDriver::avant()volatile
{
	_direction = AVANT[_noMoteur];
}

volatile void StepperDriver::derriere()volatile
{
	_direction = DERRIERE[_noMoteur];
}

volatile void StepperDriver::gauche()volatile
{
	_direction = GAUCHE[_noMoteur];
}

volatile void StepperDriver::droite()volatile
{
	_direction = DROITE[_noMoteur];
}

volatile void StepperDriver::avance()volatile
{
	_enMouvement = true;
}

volatile void StepperDriver::stop()volatile
{
	_enMouvement = false;
}

volatile const char StepperDriver::getDirection()volatile const
{
	return _direction;
}

volatile const unsigned short StepperDriver::getVitesse()volatile const
{
	for (unsigned short i = 0; i < 7; ++i)
		if (_vitesse == VITESSES[i])
			return i;
	return 8;
}

volatile bool StepperDriver::isEnMouvement()volatile const
{
	return _enMouvement;
}
