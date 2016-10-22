#include "SonarDriver.h"

SonarDriver::SonarDriver(ISonar* sonar) 
{
	_sonar = sonar;
}
SonarDriver::~SonarDriver() {}

volatile void SonarDriver::updateDist()volatile
{
	_dist = _sonar->ping_cm();
}

volatile unsigned int SonarDriver::getDist()volatile const
{
	return _dist;
}

volatile bool SonarDriver::isObstacle()volatile const
{
	return (_dist < SONAR_MIN_DIST || _dist > SONAR_MAX_DIST);
}