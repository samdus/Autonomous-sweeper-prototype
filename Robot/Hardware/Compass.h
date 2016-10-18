#ifndef COMPASS_H
#define COMPASS_H

#include "Software\ICompass.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#define DECLINATION_ANGLE 0.27
#define SENSOR_ID 12345

class Compass : ICompass
{
private:
	Adafruit_HMC5883_Unified *_mag;

public:
	Compass();
	~Compass();

	virtual void init();
	virtual float read();
};

#endif // !COMPASS_H