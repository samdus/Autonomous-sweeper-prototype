#ifndef ISONAR_H
#define ISONAR_H

class ISonar
{
public:
	volatile virtual unsigned int ping_cm()volatile = 0;
};

#endif // !ISONAR_H