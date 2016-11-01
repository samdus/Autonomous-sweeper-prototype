#ifndef COMPASS_H
#define COMPASS_H

#include "Software\ICompass.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

#define DECLINATION_ANGLE 0.27
#define SENSOR_ID 12345

class Compass : public ICompass
{
private:
	Adafruit_LSM303_Mag_Unified *_mag;
	Adafruit_LSM303_Accel_Unified *_accel;
	Adafruit_9DOF *_dof;

public:
	Compass();
	~Compass();

	 virtual bool init();
	 virtual float read();
};

#endif // !COMPASS_H