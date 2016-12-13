#ifndef ISONAR_H
#define ISONAR_H

//TODO: Trouver les distances min et max pour décider s'il y a un obstacle ou non
#define SONAR_MIN_DIST 1
#define SONAR_MAX_DIST 40

class ISonar
{
public:
	 virtual unsigned int ping_cm() = 0;
};

#endif // !ISONAR_H