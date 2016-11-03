#include "CompassDriver.h"

CompassDriver::CompassDriver( ICompass *compass)
{
	_compass = compass;
}

CompassDriver::~CompassDriver(){}

bool CompassDriver::init()
{
	return _compass->init();
}

void BubbleSort(float num[COMPAS_NB_READ])
{
	int i, j;
	float temp;
	bool flag = true;

	for (i = 1; (i <= COMPAS_NB_READ) && flag; ++i)
	{
		flag = false;
		for (j = 0; j < (COMPAS_NB_READ - 1); ++j)
		{
			if (num[j + 1] > num[j])
			{
				temp = num[j];             
				num[j] = num[j + 1];
				num[j + 1] = temp;
				flag = true;              
			}
		}
	}
}

float CompassDriver::getOrientation()
{
	float lectures[COMPAS_NB_READ];

	for (int i = 0; i < COMPAS_NB_READ; ++i)
	{
		lectures[i] = _compass->read();
	}

	BubbleSort(lectures);

	return lectures[COMPAS_NB_READ / 2];
}
