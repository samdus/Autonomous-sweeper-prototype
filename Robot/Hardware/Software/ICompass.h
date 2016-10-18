#ifndef ICOMPASS_H
#define ICOMPASS_H

class ICompass
{
public:
	virtual void init() = 0;
	virtual float read() = 0;
};

#endif // !ICOMPASS_H

