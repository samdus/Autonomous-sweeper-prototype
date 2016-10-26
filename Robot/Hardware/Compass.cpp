#include "Compass.h"

Compass::Compass()
{
	_mag = new Adafruit_HMC5883_Unified(SENSOR_ID);
}

Compass::~Compass()
{
	delete _mag;
}

 void Compass::init()
{
	_mag->begin();
}

float Compass::read()
{
	sensors_event_t event;
	_mag->getEvent(&event);

	float heading = atan2(event.magnetic.y, event.magnetic.x);
	heading += DECLINATION_ANGLE;

	// Correction des signes
	if (heading < 0)
		heading += 2 * PI;

	// La d�clinasons peut faire en sorte qu'on change de cadran
	if (heading > 2 * PI)
		heading -= 2 * PI;

	// Convertion de radian vers degr�
	return heading * 180 / M_PI;
}