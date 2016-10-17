#ifndef COMPASS_DRIVER_H
#define COMPASS_DRIVER_H

#include "ICompass.h"

#define NB_READ 100

class CompassDriver
{
private:
	ICompass *_compass;
public:
	CompassDriver(ICompass*);
	~CompassDriver();

	float getOrientation();
};

#endif // !COMPASS_DRIVER_H

