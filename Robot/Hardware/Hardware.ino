#include <Adafruit_HMC5883_U.h>
#include <Adafruit_Sensor.h>
#include <TimerOne.h>

#include "Software\ControlleurPrincipal.h"
#include "Software\CompassDriver.h"
#include "Software\SonarDriver.h"
#include "Software\StepperDriver.h"

#include "Sonar.h"
#include "Compass.h"
#include "StepperMotor.h"

#define TEMPS_TIMER1 3250
#define TEMPS_TIMER2 200

byte pinsMoteurs[STEPPER_NB_MOTEUR][4] = {
	{ 6,7,8,9 },
	{ 2,3,4,5 }
};
StepperDriver moteurGauche(new StepperMotor(), STEPPER_GAUCHE),
              moteurDroit(new StepperMotor(), STEPPER_DROIT);

SonarDriver sonarDriver(new Sonar());
CompassDriver compassDriver(new Compass());
ControlleurPrincipal controlleur(&moteurGauche, &moteurDroit, &sonarDriver, &compassDriver);

void transmettreDonnee(int donnee)
{
    Serial.write(donnee);
}

void attendre(unsigned long duree)
{
    delay(duree);
}

void stepMoteur() 
{
    controlleur.stepMoteur();
}

void validation()
{
    controlleur.verifierObstacle();
    controlleur.calibrerMoteur();
}

void setup() 
{
	Serial.begin(9600);
    
    controlleur.init(transmettreDonnee, attendre, pinsMoteurs[STEPPER_GAUCHE], pinsMoteurs[STEPPER_DROIT]);
    
	Timer1.initialize(TEMPS_TIMER1);
	Timer1.attachInterrupt(stepMoteur);
    
    Timer1.initialize(TEMPS_TIMER2);
    Timer1.attachInterrupt(validation);
}

void loop()
{
    if (Serial.available() > 0) {
        switch (Serial.read())
        {
        case ControlleurPrincipal::Fonction::Avance:
            Serial.write(controlleur.avancePendantXDixiemeSec(Serial.read()));
            break;

        case ControlleurPrincipal::Fonction::Recule:
            Serial.write(controlleur.reculePendantXDixiemeSec(Serial.read()));
            break;

        case ControlleurPrincipal::Fonction::Stop:
            Serial.write(controlleur.stop());
            break;

        case ControlleurPrincipal::Fonction::Tourne:
            Serial.write(controlleur.tourneAuDegresX(Serial.read()));
            break;

        case ControlleurPrincipal::Fonction::Orientation:
            Serial.write(controlleur.tourneAuDegresX(Serial.read()));
            break;

        case ControlleurPrincipal::Fonction::Gauche:
            Serial.write(controlleur.tourneGauche(Serial.read()));
            break;
            
        case ControlleurPrincipal::Fonction::Droite:
            Serial.write(controlleur.tourneDroite(Serial.read()));
            break;

        case ControlleurPrincipal::Fonction::SetDebug:
            controlleur.setDebug();
            break;
            
        case ControlleurPrincipal::Fonction::StopDebug:
            controlleur.stopDebug();
            break;
        }
    }
    /*
	nouvOrientation = compassDriver.getOrientation();
	Serial.print("Orientation: ");
	Serial.print(nouvOrientation);
	Serial.println(" degres.");

	//Si on va vers la gauche
	if (moteurGauche.getDirection() == 1 && moteurDroit.getDirection() == -1)
	{
		if (nouvOrientation >= getDestination(orientation, 90, true))
		{
			moteurGauche.droite();
			moteurDroit.droite();

			orientation = nouvOrientation;
		}
	}
	else
	{
		if (nouvOrientation <= getDestination(orientation, 90, false))
		{
			moteurGauche.gauche();
			moteurDroit.gauche();

			orientation = nouvOrientation;
		}
	}
    */
	/*if (nouvOrientation >= orientation + 89  ||
		nouvOrientation > orientation - 271 && nouvOrientation < orientation - 269 ||
		nouvOrientation > orientation - 89 && nouvOrientation < orientation - 91 ||
		nouvOrientation > orientation + 271 && nouvOrientation < orientation + 269)
	{
		if (moteurGauche.getDirection() == 1 && moteurDroit.getDirection() == -1)
		{
			moteurGauche.droite();
			moteurDroit.droite();
		}
		else
		{
			moteurGauche.gauche();
			moteurDroit.gauche();
		}
		orientation = nouvOrientation;
	}*/
	/*if (Serial.available() > 0) {
		switch (Serial.read())
		{
		case 'A':
			Serial.println("Avance!");

			moteurGauche.avant();
			moteurDroit.avant();
						
			moteurGauche.avance();
			moteurDroit.avance();
			break;
		case 'R':
			Serial.println("Recule!");
			moteurGauche.derriere();
			moteurDroit.derriere();

			moteurGauche.avance();
			moteurDroit.avance();
			break;
		case 'D':
			Serial.println("Droite!");
			moteurGauche.droite();
			moteurDroit.droite();

			moteurGauche.avance();
			moteurDroit.avance();
			break;
		case 'G':
			Serial.println("Gauche!");
			moteurGauche.gauche();
			moteurDroit.gauche();

			moteurGauche.avance();
			moteurDroit.avance();
			break; 
		case 'S':
			Serial.println("STOP!");
			moteurGauche.stop();
			moteurDroit.stop();
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
		moteurGauche.stop();
		moteurDroit.stop();
	}

	/*Serial.print("Orientation: ");
	Serial.print(compassDriver.getOrientation());
	Serial.println(" degres.");

	Serial.println();

	Serial.flush();*/


}