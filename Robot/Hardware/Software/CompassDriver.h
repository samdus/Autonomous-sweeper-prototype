#ifndef COMPASS_DRIVER_H
#define COMPASS_DRIVER_H

#include "ICompass.h"

#define COMPAS_NB_READ 10

class CompassDriver
{
private:
	volatile ICompass *_compass;
public:
	CompassDriver(volatile ICompass*);
	~CompassDriver();
	
	volatile virtual void init()volatile;
	volatile virtual float getOrientation()volatile;
};

#endif // !COMPASS_DRIVER_H

