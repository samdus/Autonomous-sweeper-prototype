#ifndef COMPASS_DRIVER_H
#define COMPASS_DRIVER_H

#include "ICompass.h"

#define NB_READ 10

class CompassDriver
{
private:
	ICompass *_compass;
public:
	CompassDriver(ICompass*);
	~CompassDriver();

	void init();
	float getOrientation();
};

#endif // !COMPASS_DRIVER_H

