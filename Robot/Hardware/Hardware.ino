#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "Software\SonarDriver.h"

#include "Sonar.h"
#include "Compass.h"
/*
SonarDriver sonarDriver(new Sonar());
Compass compass;

void setup()
{
	Serial.begin(9600);
	compass.init();
}

void loop()
{
	/*
	TEST pour le sonar
	-------------------------------------------------
	Serial.print("Distance: ");

	sonarDriver.updateDist();

	if (sonarDriver.getDist() < 10)
		Serial.print(0);

	Serial.print(sonarDriver.getDist());
	Serial.println(" cm.");

	if (sonarDriver.isObstacle())
	{
		Serial.println("STOP!!!");
	}
	
	delay(10);
	/

	/*
	TEST pour le compass
	-------------------------------------------------
	Serial.print("Orientation: ");
	Serial.print(compass.read());
	Serial.println(" degres.");
	/
}
*/

#include <TimerOne.h>
#include "StepperMotor.h"

#define NB_MOTEUR 2 
#define GAUCHE 0
#define DROIT 1

#define TEMPS_TIMER1 3250

byte pinsMoteurs[NB_MOTEUR][4] = {
	{ 2,3,4,5 },
	{ 6,7,8,9 }
};
unsigned short vitesses[9] =
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
StepperMotor moteurs[NB_MOTEUR];

char avant[NB_MOTEUR] = { -1,-1 },
derriere[NB_MOTEUR] = { 1, 1 },
gauche[NB_MOTEUR] = { 1,-1 },
droite[NB_MOTEUR] = { -1, 1 };

char* directionActuelle = avant;

unsigned short vitesseMoteurs[NB_MOTEUR] = { vitesses[8], vitesses[8] };
unsigned short compteursMoteur[NB_MOTEUR] = { 1, 1 };

byte i;

void stepMoteur() {
	for (i = 0; i < NB_MOTEUR; ++i) {
		if ((vitesseMoteurs[i] & compteursMoteur[i]) > 0)
		{
			moteurs[i].nextStep(directionActuelle[i]);
		}

		if ((compteursMoteur[i] <<= 1) == 0) {
			compteursMoteur[i] = 1;
		}
	}
}

void setup() {
	Serial.begin(9600);
	moteurs[GAUCHE].init(pinsMoteurs[GAUCHE][0], pinsMoteurs[GAUCHE][2], pinsMoteurs[GAUCHE][1], pinsMoteurs[GAUCHE][3]);
	moteurs[DROIT].init(pinsMoteurs[DROIT][0], pinsMoteurs[DROIT][2], pinsMoteurs[DROIT][1], pinsMoteurs[DROIT][3]);

	Timer1.initialize(TEMPS_TIMER1);
	Timer1.attachInterrupt(stepMoteur);
}

char m, v[1];
void loop() {
	if (Serial.available() > 0) {
		switch (Serial.read())
		{
		case 'A':
			Serial.println("Avance!");
			directionActuelle = avant;
			break;
		case 'R':
			Serial.println("Recule!");
			directionActuelle = derriere;
			break;
		case 'D':
			Serial.println("Droite!");
			directionActuelle = droite;
			break;
		case 'G':
			Serial.println("Gauche!");
			directionActuelle = gauche;
			break;
		case 'V':
			delay(2);
			m = Serial.read();
			delay(2);
			v[0] = Serial.read();

			Serial.print("Moteur: ");
			Serial.print(m);
			Serial.print(" --> Vitesse: ");
			Serial.println(atoi(v));

			vitesseMoteurs[m == 'D'] = vitesses[atoi(v)];
			break;
		default:
			Serial.println("Comprends pas...");
		}
	}

	delay(1000);
}