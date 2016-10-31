#include "Compass.h"

Compass::Compass()
{
	_mag = new Adafruit_LSM303_Mag_Unified(30302);
	_accel = new Adafruit_LSM303_Accel_Unified(30301);
	_dof = new Adafruit_9DOF();
}

Compass::~Compass()
{
	delete _mag;
}

bool Compass::init()
{
	return _accel->begin() && _mag->begin();
}

float Compass::read()
{
	sensors_event_t accel_event;
	sensors_event_t mag_event;
	sensors_vec_t   orientation;

	_accel->getEvent(&accel_event);
	_mag->getEvent(&mag_event);

	_dof->fusionGetOrientation(&accel_event, &mag_event, &orientation);

	return orientation.heading;
}