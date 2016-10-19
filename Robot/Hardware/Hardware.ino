#include <TimerOne.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#include "Software\CompassDriver.h"
#include "Software\SonarDriver.h"
#include "Software\StepperDriver.h"

#include "Sonar.h"
#include "Compass.h"
#include "StepperMotor.h"

#define NB_MOTEUR 2 
#define GAUCHE 0
#define DROIT 1

#define TEMPS_TIMER1 3250

byte pinsMoteurs[NB_MOTEUR][4] = {
	{ 6,7,8,9 },
	{ 2,3,4,5 }
};
StepperDriver moteursDrivers[NB_MOTEUR] = { StepperDriver(new StepperMotor(), GAUCHE), StepperDriver(new StepperMotor(), DROIT) };

SonarDriver sonarDriver(new Sonar());
CompassDriver compassDriver(new Compass());

float orientation, nouvOrientation;

void stepMoteur() {
	moteursDrivers[GAUCHE].step();
	moteursDrivers[DROIT].step();
}

void setup() {
	Serial.begin(9600);
	
	moteursDrivers[GAUCHE].init(pinsMoteurs[GAUCHE][0], pinsMoteurs[GAUCHE][2], pinsMoteurs[GAUCHE][1], pinsMoteurs[GAUCHE][3]);
	moteursDrivers[DROIT].init(pinsMoteurs[DROIT][0], pinsMoteurs[DROIT][2], pinsMoteurs[DROIT][1], pinsMoteurs[DROIT][3]);

	compassDriver.init();

	Timer1.initialize(TEMPS_TIMER1);
	Timer1.attachInterrupt(stepMoteur);

	orientation = compassDriver.getOrientation();

	moteursDrivers[GAUCHE].gauche();
	moteursDrivers[DROIT].gauche();

	moteursDrivers[GAUCHE].avance();
	moteursDrivers[DROIT].avance();
}

char moteur, nouvelleVitesse[1];

float getDestination(float depart, float angle, bool gauche)
{
	float resultat;

	if (gauche)
	{
		resultat = depart + angle;

		if (resultat > 360)
			resultat -= 360;
	}
	else
	{
		resultat = depart - angle;

		if (resultat < 0)
			resultat += 360;
	}

	return resultat;
}

void loop() 
{
	nouvOrientation = compassDriver.getOrientation();
	Serial.print("Orientation: ");
	Serial.print(nouvOrientation);
	Serial.println(" degres.");

	//Si on va vers la gauche
	if (moteursDrivers[GAUCHE].getDirection() == 1 && moteursDrivers[DROIT].getDirection() == -1)
	{
		if (nouvOrientation >= getDestination(orientation, 90, true))
		{
			moteursDrivers[GAUCHE].droite();
			moteursDrivers[DROIT].droite();

			orientation = nouvOrientation;
		}
	}
	else
	{
		if (nouvOrientation <= getDestination(orientation, 90, false))
		{
			moteursDrivers[GAUCHE].gauche();
			moteursDrivers[DROIT].gauche();

			orientation = nouvOrientation;
		}
	}
	/*if (nouvOrientation >= orientation + 89  ||
		nouvOrientation > orientation - 271 && nouvOrientation < orientation - 269 ||
		nouvOrientation > orientation - 89 && nouvOrientation < orientation - 91 ||
		nouvOrientation > orientation + 271 && nouvOrientation < orientation + 269)
	{
		if (moteursDrivers[GAUCHE].getDirection() == 1 && moteursDrivers[DROIT].getDirection() == -1)
		{
			moteursDrivers[GAUCHE].droite();
			moteursDrivers[DROIT].droite();
		}
		else
		{
			moteursDrivers[GAUCHE].gauche();
			moteursDrivers[DROIT].gauche();
		}
		orientation = nouvOrientation;
	}*/
	/*if (Serial.available() > 0) {
		switch (Serial.read())
		{
		case 'A':
			Serial.println("Avance!");

			moteursDrivers[GAUCHE].avant();
			moteursDrivers[DROIT].avant();
						
			moteursDrivers[GAUCHE].avance();
			moteursDrivers[DROIT].avance();
			break;
		case 'R':
			Serial.println("Recule!");
			moteursDrivers[GAUCHE].derriere();
			moteursDrivers[DROIT].derriere();

			moteursDrivers[GAUCHE].avance();
			moteursDrivers[DROIT].avance();
			break;
		case 'D':
			Serial.println("Droite!");
			moteursDrivers[GAUCHE].droite();
			moteursDrivers[DROIT].droite();

			moteursDrivers[GAUCHE].avance();
			moteursDrivers[DROIT].avance();
			break;
		case 'G':
			Serial.println("Gauche!");
			moteursDrivers[GAUCHE].gauche();
			moteursDrivers[DROIT].gauche();

			moteursDrivers[GAUCHE].avance();
			moteursDrivers[DROIT].avance();
			break; 
		case 'S':
			Serial.println("STOP!");
			moteursDrivers[GAUCHE].stop();
			moteursDrivers[DROIT].stop();
			break;
		case 'V':
			delay(2);
			moteur = Serial.read();
			delay(2);
			nouvelleVitesse[0] = Serial.read();

			Serial.print("Moteur: ");
			Serial.print(moteur);
			Serial.print(" --> Vitesse: ");
			Serial.println(atoi(nouvelleVitesse));

			moteursDrivers[moteur == 'D'].setVitesse(atoi(nouvelleVitesse));
			break;
		default:
			Serial.println("Comprends pas...");
		}
	}
	
	Serial.print("Distance: ");
	
	sonarDriver.updateDist();
	
	if (sonarDriver.getDist() < 10)
		Serial.print(0);

	Serial.print(sonarDriver.getDist());
	Serial.println(" cm.");
	
	if (sonarDriver.isObstacle())
	{
		//Serial.println("STOP!!!");
		moteursDrivers[GAUCHE].stop();
		moteursDrivers[DROIT].stop();
	}

	/*Serial.print("Orientation: ");
	Serial.print(compassDriver.getOrientation());
	Serial.println(" degres.");

	Serial.println();

	Serial.flush();*/


}