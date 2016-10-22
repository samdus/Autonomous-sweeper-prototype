#ifndef SONAR_DRIVER_H
#define SONAR_DRIVER_H

#include "ISonar.h"

#define SONAR_MIN_DIST 6
#define SONAR_MAX_DIST 15

class SonarDriver
{
private:
	volatile ISonar *_sonar;
	volatile unsigned int _dist = 0;

public:
	SonarDriver(ISonar*);
	~SonarDriver();

	volatile virtual void updateDist()volatile;

	volatile virtual unsigned int getDist()volatile const;
	volatile virtual bool isObstacle()volatile const;
};

#endif // !SONAR_DRIVER_H

