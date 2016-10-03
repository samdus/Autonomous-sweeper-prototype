#include "SonarDriver.h"

SonarDriver::SonarDriver(ISonar* sonar) 
{
	_sonar = sonar;
}
SonarDriver::~SonarDriver() {}

void SonarDriver::updateDist()
{
	_dist = _sonar->ping_cm();
}

unsigned int SonarDriver::getDist()
{
	return _dist;
}

bool SonarDriver::isObstacle()
{
	return (_dist < 5 || _dist > 10);
}