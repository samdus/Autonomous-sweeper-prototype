#ifndef SONAR_DRIVER_H
#define SONAR_DRIVER_H

#include "ISonar.h"

class SonarDriver
{
private:
	ISonar *sonar;
public:
	SonarDriver();
	~SonarDriver();


};

#endif // !SONAR_DRIVER_H

