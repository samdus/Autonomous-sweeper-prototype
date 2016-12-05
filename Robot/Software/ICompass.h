#ifndef ICOMPASS_H
#define ICOMPASS_H

class ICompass
{
public:
	 virtual bool init() = 0;
	 virtual float read() = 0;
	 virtual void update() = 0;
};

#endif // !ICOMPASS_H

