#include "StepperDriver.h"

StepperDriver::StepperDriver( IStepper* stepper, unsigned short noMoteur)
{
	_noMoteur = noMoteur;
	_stepper = stepper;
	_compteur = 1;
	_enMouvement = false;
	_direction = AVANT[_noMoteur];
	_vitesse = VITESSES[8];
}

StepperDriver::~StepperDriver(){}

 void StepperDriver::init(byte mPin_1, byte mPin_2, byte mPin_3, byte mPin_4) 
{
	_stepper->init(mPin_1, mPin_2, mPin_3, mPin_4);
}

 void StepperDriver::step()
{
	if (_enMouvement)
	{
		if ((_vitesse & _compteur) > 0)
			_stepper->nextStep(_direction);

		if ((_compteur <<= 1) == 0)
			_compteur = 1;
	}
}

 void StepperDriver::setVitesse(unsigned short vitesse)
{
	if (vitesse > 8)
	{
		vitesse = 8;
	}

	_vitesse = VITESSES[vitesse];
}

 void StepperDriver::avant()
{
	_direction = AVANT[_noMoteur];
}

 void StepperDriver::derriere()
{
	_direction = DERRIERE[_noMoteur];
}

 void StepperDriver::gauche()
{
	_direction = GAUCHE[_noMoteur];
}

 void StepperDriver::droite()
{
	_direction = DROITE[_noMoteur];
}

 void StepperDriver::avance()
{
	setVitesse(8);
	_enMouvement = true;
}

 void StepperDriver::stop()
{
	_enMouvement = false;
}

 const char StepperDriver::getDirection() const
{
	return _direction;
}

 const unsigned short StepperDriver::getVitesse() const
{
	for (unsigned short i = 0; i < 8; ++i)
	{
		if (_vitesse == VITESSES[i])
			return i;
	}
	return 8;
}

bool StepperDriver::isEnMouvement() const
{
	return _enMouvement;
}
