#include "CompassDriver.h"

CompassDriver::CompassDriver( ICompass *compass)
{
	_compass = compass;
}

CompassDriver::~CompassDriver(){}

bool CompassDriver::init()
{
	return _compass->init();
}

 float CompassDriver::getOrientation()
{
	float avg = 0;

	for (int i = 0; i < COMPAS_NB_READ; ++i)
	{
		avg += _compass->read();
	}

	return avg / COMPAS_NB_READ;
}
