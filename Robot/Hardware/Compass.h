#ifndef COMPASS_H
#define COMPASS_H

#include "Software\ICompass.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <MadgwickAHRS.h>
#include <elapsedMillis.h>

#define DECLINATION_ANGLE 0.27
#define SENSOR_ID 12345

#define COMPASS_REFRESH_RATE_MS 270
#define COMPASS_NB_REFRESH_PER_SEC (1000 / COMPASS_REFRESH_RATE_MS)

class Compass : public ICompass
{
private:
	Adafruit_L3GD20_Unified       *_gyro;
	Adafruit_LSM303_Accel_Unified *_accel;
	Adafruit_LSM303_Mag_Unified   *_mag;

	//elapsedMillis timeElapsed = 0;

	Madgwick *_filter;

	// Offsets applied to raw x/y/z values
	const float mag_offsets[3] = { -3.99F, -5.93F, -10.85F };

	// Soft iron error compensation matrix
	const float mag_softiron_matrix[3][3] = { { 0.932, 0.007, 0.007 },
											  { 0.007, 0.936, 0.002 },
											  { 0.007, 0.002, 1.147 } };

	const float mag_field_strength = 51.46F;

public:
	Compass();
	~Compass();

	 virtual bool init();
	 virtual float read();
	 virtual void update();
};

#endif // !COMPASS_H