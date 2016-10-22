#ifndef SONAR_H
#define SONAR_H

#define ECHO_PIN     11  
#define TRIGGER_PIN  10
#define MAX_DISTANCE 200

#define NB_PING 20
#include "Software/ISonar.h"
#include <NewPing.h>

class Sonar : public ISonar
{
private:
	NewPing* _sonar;

public:
	Sonar();
	~Sonar();

	volatile virtual unsigned int ping_cm()volatile;
};

#endif // SONAR_H

