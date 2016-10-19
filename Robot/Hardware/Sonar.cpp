#include "Sonar.h"

Sonar::Sonar()
{
	_sonar = new NewPing(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
}

Sonar::~Sonar()
{
	delete _sonar;
}

unsigned int Sonar::ping_cm()
{
	return _sonar->convert_cm(_sonar->ping_median(NB_PING));
}