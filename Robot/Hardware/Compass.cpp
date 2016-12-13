#include "Compass.h"

Compass::Compass()
{
	_gyro = new Adafruit_L3GD20_Unified(20);
	_mag = new Adafruit_LSM303_Mag_Unified(30302);
	_accel = new Adafruit_LSM303_Accel_Unified(30301);

	_filter = new Madgwick();
}

Compass::~Compass()
{
	delete _mag;
}

bool Compass::init()
{
	_filter->begin(COMPASS_NB_REFRESH_PER_SEC);
	return _gyro->begin() && _accel->begin() && _mag->begin();
}

float Compass::read()
{
	return _filter->getYaw();
}

void Compass::update()
{
	sensors_event_t gyro_event;
	sensors_event_t accel_event;
	sensors_event_t mag_event;

	//TODO: Dynamiser le refresh rate
	/*_filter->begin(1000 / timeElapsed);
	Serial.print("Temps: ");
	Serial.println(timeElapsed);
	timeElapsed = 0;*/
	_gyro->getEvent(&gyro_event);
	_accel->getEvent(&accel_event);
	_mag->getEvent(&mag_event);

	// Appliquer la compensation pour le magnetometre
	float x = mag_event.magnetic.x - mag_offsets[0];
	float y = mag_event.magnetic.y - mag_offsets[1];
	float z = mag_event.magnetic.z - mag_offsets[2];

	// Appliquer la compensation pour l'erreur d'interferance (soft iron)
	float mx = x * mag_softiron_matrix[0][0] + y * mag_softiron_matrix[0][1] + z * mag_softiron_matrix[0][2];
	float my = x * mag_softiron_matrix[1][0] + y * mag_softiron_matrix[1][1] + z * mag_softiron_matrix[1][2];
	float mz = x * mag_softiron_matrix[2][0] + y * mag_softiron_matrix[2][1] + z * mag_softiron_matrix[2][2];

	// Transfert de radian vers degres
	float gx = gyro_event.gyro.x * 57.2958F;
	float gy = gyro_event.gyro.y * 57.2958F;
	float gz = gyro_event.gyro.z * 57.2958F;
	
	// Mettre a jour a l'aide de l'algorithme de filtre (compense les donneees du gyroscope avec ceux des autres capteurs)
	_filter->update(gx, gy, gz,
					accel_event.acceleration.x, accel_event.acceleration.y, accel_event.acceleration.z,
					mx, my, mz);
}