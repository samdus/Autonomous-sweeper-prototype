#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#include "IStepper.h"

#define NB_MOTEUR 2 

class StepperDriver 
{
private:
	const unsigned short VITESSES[9] =
	{
		43690, //1010 1010 1010 1010 ( 8/16)
		54618, //1101 0101 0101 1010 ( 9/16)
		54966, //1101 0110 1011 0110 (10/16)
		56173, //1101 1011 0110 1101 (11/16)
		61166, //1110 1110 1110 1110 (12/16)
		61309, //1110 1111 0111 1101 (13/16)
		61423, //1110 1111 1110 1111 (14/16)
		65407, //1111 1111 0111 1111 (15/16)
		65535, //1111 1111 1111 1111 (16/16)
	};
	const char AVANT   [NB_MOTEUR] = { -1, -1 },
			   DERRIERE[NB_MOTEUR] = {  1,  1 },
			   DROITE  [NB_MOTEUR] = { -1,  1 },
			   GAUCHE  [NB_MOTEUR] = {  1, -1 };

	unsigned short _compteur;
	char _direction;
	bool _enMouvement;
	unsigned short _noMoteur;
	unsigned short _vitesse;
	
	IStepper*   _stepper;	
public:
	StepperDriver(IStepper*, unsigned short);
	~StepperDriver();

	void init(byte, byte, byte, byte);

	void step();
	void setVitesse(unsigned short);

	void avant();
	void derriere();
	void gauche();
	void droite();

	void avance();
	void stop();

	const char getDirection();
	const unsigned short getVitesse();
	bool isEnMouvement();
};

#endif // !STEPPER_DRIVER_H
