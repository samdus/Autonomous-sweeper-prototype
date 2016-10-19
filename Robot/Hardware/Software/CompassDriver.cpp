#include "CompassDriver.h"

CompassDriver::CompassDriver(ICompass *compass)
{
	_compass = compass;
}

CompassDriver::~CompassDriver(){}

void CompassDriver::init()
{
	_compass->init();
}

float CompassDriver::getOrientation()
{
	float avg = 0;

	for (int i = 0; i < NB_READ; ++i)
	{
		avg += _compass->read();
	}

	return avg / NB_READ;
}
