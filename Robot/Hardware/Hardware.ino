#include <MsTimer2.h>
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
#define TEMPS_TIMER2 500

byte pinsMoteurs[STEPPER_NB_MOTEUR][4] = {
	{ 6,7,8,9 },
	{ 2,3,4,5 }
};
volatile StepperDriver moteurGauche(new StepperMotor(), STEPPER_GAUCHE),
                       moteurDroit(new StepperMotor(), STEPPER_DROIT);

volatile SonarDriver sonarDriver(new Sonar());
volatile CompassDriver compassDriver(new Compass());
volatile ControlleurPrincipal controlleur(&moteurGauche, &moteurDroit, &sonarDriver, &compassDriver);

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

	MsTimer2::set(TEMPS_TIMER2, validation);
	MsTimer2::start();

	Serial.println(controlleur.obtenirOrientation());
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
            Serial.write(controlleur.obtenirOrientation());
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
		default:
			Serial.write(ControlleurPrincipal::Fonction::Erreur);
			Serial.write(ControlleurPrincipal::TypeErreur::FonctionInconnue);
        }
		Serial.flush();
    }
}