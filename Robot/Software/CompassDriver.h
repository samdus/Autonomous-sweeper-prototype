#ifndef COMPASS_DRIVER_H
#define COMPASS_DRIVER_H

#include "ICompass.h"

#define COMPAS_NB_READ 10

class CompassDriver
{
private:
	 ICompass *_compass;
public:
	CompassDriver(ICompass*);
	~CompassDriver();
	
	virtual bool init();
	virtual void update();
	virtual float getOrientation();
};

#endif // !COMPASS_DRIVER_H

