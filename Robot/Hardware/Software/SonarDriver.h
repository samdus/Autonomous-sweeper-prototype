#ifndef SONAR_DRIVER_H
#define SONAR_DRIVER_H

#include "ISonar.h"

#define SONAR_MIN_DIST 6
#define SONAR_MAX_DIST 15

class SonarDriver
{
private:
	 ISonar *_sonar;
	 unsigned int _dist = 0;

public:
	SonarDriver(ISonar*);
	~SonarDriver();

	 virtual void updateDist();

	 virtual unsigned int getDist() const;
	 virtual bool isObstacle() const;
};

#endif // !SONAR_DRIVER_H

