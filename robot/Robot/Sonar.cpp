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
	unsigned int dist_tmp = 0;

	for (byte i = 0; i < NB_PING; i++)
	{
		dist_tmp = _sonar->ping_cm() + dist_tmp;
		delay(20);
	}

	return dist_tmp / (NB_PING - 1);
}