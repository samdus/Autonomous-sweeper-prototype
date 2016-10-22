#include "CompassDriver.h"

CompassDriver::CompassDriver(volatile ICompass *compass)
{
	_compass = compass;
}

CompassDriver::~CompassDriver(){}

volatile void CompassDriver::init()volatile
{
	_compass->init();
}

volatile float CompassDriver::getOrientation()volatile
{
	float avg = 0;

	for (int i = 0; i < COMPAS_NB_READ; ++i)
	{
		avg += _compass->read();
	}

	return avg / COMPAS_NB_READ;
}
