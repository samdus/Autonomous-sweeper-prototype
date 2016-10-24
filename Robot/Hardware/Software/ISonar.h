#ifndef ISONAR_H
#define ISONAR_H

class ISonar
{
public:
	 virtual unsigned int ping_cm() = 0;
};

#endif // !ISONAR_H