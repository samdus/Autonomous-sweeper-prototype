#ifndef SONAR_DRIVER_H
#define SONAR_DRIVER_H

#include "ISonar.h"

class SonarDriver
{
private:
	ISonar *_sonar;
	unsigned int _dist = 0;

public:
	SonarDriver(ISonar*);
	~SonarDriver();

	void updateDist();

	unsigned int getDist();
	bool isObstacle();
};

#endif // !SONAR_DRIVER_H

