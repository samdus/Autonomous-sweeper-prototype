#ifndef ICOMPASS_H
#define ICOMPASS_H

class ICompass
{
public:
	volatile virtual void init()volatile = 0;
	volatile virtual float read()volatile = 0;
};

#endif // !ICOMPASS_H

