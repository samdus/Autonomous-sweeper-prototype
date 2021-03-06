#include "Sonar.h"

Sonar::Sonar()
{
	_sonar = new NewPing(TRIGGER_PIN, ECHO_PIN, SONAR_MAX_DIST);
}

Sonar::~Sonar()
{
	delete _sonar;
}

 unsigned int Sonar::ping_cm()
{
	return _sonar->convert_cm(_sonar->ping_median(NB_PING));
}