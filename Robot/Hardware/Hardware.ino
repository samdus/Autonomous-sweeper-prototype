#include <elapsedMillis.h>
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

byte pinsMoteurs[STEPPER_NB_MOTEUR][4] = {
	{ 6,7,8,9 },
	{ 2,3,4,5 }
};
StepperDriver moteurGauche(new StepperMotor(), STEPPER_GAUCHE),
              moteurDroit(new StepperMotor(), STEPPER_DROIT);
SonarDriver sonarDriver(new Sonar());
CompassDriver compassDriver(new Compass());
ControlleurPrincipal controlleur(&moteurGauche, &moteurDroit, &sonarDriver, &compassDriver);

void ecrireInt(int aEcrire)
{
	uint8_t *donnees = new uint8_t[2];

	donnees[0] = (aEcrire << 8) >> 8;
	donnees[1] = aEcrire >> 8;

	Serial.write(donnees, 2);

	delete donnees;
}

int lireInt()
{
	uint8_t *donnees = new uint8_t[2];
	int resultat;

	Serial.readBytes(donnees, 2);

	resultat = (donnees[1] << 8) | donnees[0];

	delete donnees;
	return resultat;
}

void transmettreDonnee(int donnee, bool entier)
{
	if (entier)
		ecrireInt(donnee);
	else
		Serial.write(donnee);
}

void attendre(unsigned long duree)
{
    elapsedMillis timeElapsed = 0;
    while (timeElapsed < duree)
    {
		controlleur.verifierObstacle();
		controlleur.calibrerMoteur();
    }
}

void stepMoteur()
{
    controlleur.stepMoteur();
}

void setup() 
{
	Serial.begin(9600);
    
    controlleur.init(transmettreDonnee, attendre, pinsMoteurs[STEPPER_GAUCHE], pinsMoteurs[STEPPER_DROIT]);
    
	Timer1.initialize(TEMPS_TIMER1);
	Timer1.attachInterrupt(stepMoteur);
}

void loop()
{
    if (Serial.available() > 0) {
		int retour = 0;
        switch (Serial.read())
        {
        case ControlleurPrincipal::Fonction::Avance:
			retour = controlleur.avancePendantXDixiemeSec(lireInt());

			Serial.write(ControlleurPrincipal::Fonction::RetourBool);
            Serial.write(retour);
            break;

        case ControlleurPrincipal::Fonction::Recule:
			retour = controlleur.reculePendantXDixiemeSec(lireInt());

			Serial.write(ControlleurPrincipal::Fonction::RetourBool);
			Serial.write(retour);
            break;

        case ControlleurPrincipal::Fonction::Tourne:
			retour = controlleur.tourneAuDegresX(lireInt());

			Serial.write(ControlleurPrincipal::Fonction::RetourBool);
			Serial.write(retour);
            break;

		case ControlleurPrincipal::Fonction::GauchePendant:
			retour = controlleur.tourneGauchePendant(lireInt());

			Serial.write(ControlleurPrincipal::Fonction::RetourBool);
			Serial.write(retour);
			break;

		case ControlleurPrincipal::Fonction::DroitePendant:
			retour = controlleur.tourneDroitePendant(lireInt());

			Serial.write(ControlleurPrincipal::Fonction::RetourBool);
			Serial.write(retour);
			break;

        case ControlleurPrincipal::Fonction::Orientation:
            retour = controlleur.obtenirOrientation();

			Serial.write(ControlleurPrincipal::Fonction::RetourInt);
			ecrireInt(retour);
            break;

        case ControlleurPrincipal::Fonction::Gauche:
			retour = controlleur.tourneGauche(lireInt());

			Serial.write(ControlleurPrincipal::Fonction::RetourBool);
			Serial.write(retour);
            break;
            
        case ControlleurPrincipal::Fonction::Droite:
			retour = controlleur.tourneDroite(lireInt());

			Serial.write(ControlleurPrincipal::Fonction::RetourBool);
			Serial.write(retour);
            break;

        case ControlleurPrincipal::Fonction::SetDebug:
            controlleur.setDebug();
            break;
            
        case ControlleurPrincipal::Fonction::StopDebug:
            controlleur.stopDebug();
            break;

		default:
			Serial.write(ControlleurPrincipal::Fonction::Erreur);
			ecrireInt(ControlleurPrincipal::TypeErreur::FonctionInconnue);
        }
		Serial.flush();
    }
}