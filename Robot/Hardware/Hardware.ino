#include <elapsedMillis.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include <MadgwickAHRS.h>
#include <MahonyAHRS.h>
#include <TimerOne.h>

#include "Software\ControlleurPrincipal.h"
#include "Software\CompassDriver.h"
#include "Software\SonarDriver.h"
#include "Software\StepperDriver.h"

#include "Sonar.h"
#include "Compass.h"
#include "StepperMotor.h"

#define TEMPS_TIMER1 3250

elapsedMillis timeElapsed = 0;
int* retourDeFonction = NULL;
bool retourInt;
void(*fonctionAsync)(ControlleurPrincipal&);

byte pinsMoteurs[STEPPER_NB_MOTEUR][4] = {
	{ 6,7,8,9 },
	{ 2,3,4,5 }
};
StepperDriver moteurGauche(new StepperMotor(), STEPPER_GAUCHE),
              moteurDroit(new StepperMotor(), STEPPER_DROIT);
Compass compas;

SonarDriver sonarDriver(new Sonar());
CompassDriver compassDriver(&compas);
ControlleurPrincipal controlleur(&moteurGauche, &moteurDroit, &sonarDriver, &compassDriver, &retourDeFonction, &fonctionAsync);

void ecrireInt(int16_t aEcrire)
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

void resetTemps()
{
	timeElapsed = 0;
}

unsigned long obtenirTemps()
{
	return timeElapsed;
}

void stepMoteur()
{
    controlleur.stepMoteur();
}

void setup() 
{
	Serial.begin(9600);
    
	while (!Serial); //On attend que le port serie soit connecte

	if (!controlleur.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurs[STEPPER_GAUCHE], pinsMoteurs[STEPPER_DROIT]))
	{		
		Serial.write(IControlleurPrincipal::Fonction::Erreur);
		ecrireInt(IControlleurPrincipal::TypeErreur::ErreurInitialisation);

		while(1); //Boucle infini pour ne pas continuer
	}
    
	Timer1.initialize(TEMPS_TIMER1);
	Timer1.attachInterrupt(stepMoteur);

	Serial.write(IControlleurPrincipal::Fonction::FinInit);
	Serial.println("FIN debut");
}

unsigned long dernierTemps = 0;
unsigned long tot_DiffTemps = 0;
int nbEchantillon = 0;

void loop()
{
	controlleur.verifierObstacle();
	controlleur.calibrerMoteur();
	compas.update();
	
	retourInt = true;
	controlleur.obtenirOrientation();

	if (retourDeFonction != NULL)
	{
		if (retourInt)
		{
			Serial.write(IControlleurPrincipal::Fonction::RetourInt);
			ecrireInt(*retourDeFonction);
		}
		else
		{
			Serial.write(IControlleurPrincipal::Fonction::RetourBool);
			Serial.write(*retourDeFonction);
		}
		Serial.flush();

		delete retourDeFonction;
		retourDeFonction = NULL;
	}

	if (fonctionAsync != NULL)
	{
		fonctionAsync(controlleur);
	}

    if (fonctionAsync == NULL && Serial.available() > 0) {
        switch (Serial.read())
        {
		case IControlleurPrincipal::Fonction::FinInit:
			Serial.write(IControlleurPrincipal::Fonction::FinInit);
			break;

        case IControlleurPrincipal::Fonction::Avance:
			retourInt = false;
			controlleur.avancePendantXDixiemeSec(lireInt());
            break;

        case IControlleurPrincipal::Fonction::Recule:
			retourInt = false;
			controlleur.reculePendantXDixiemeSec(lireInt());
            break;

		case IControlleurPrincipal::Fonction::Tourne:
			retourInt = false;
			controlleur.tourneAuDegresX(lireInt());
            break;

		case IControlleurPrincipal::Fonction::GauchePendant:
			retourInt = false;
			controlleur.tourneGauchePendant(lireInt());
			break;

		case IControlleurPrincipal::Fonction::DroitePendant:
			retourInt = false;
			controlleur.tourneDroitePendant(lireInt());
			break;

		case 'D':
        case IControlleurPrincipal::Fonction::Orientation:
			retourInt = true;
            controlleur.obtenirOrientation();
            break;

        case IControlleurPrincipal::Fonction::Gauche:
			retourInt = false;
			controlleur.tourneGauche(lireInt());
            break;
            
        case IControlleurPrincipal::Fonction::Droite:
			retourInt = false;
			controlleur.tourneDroite(lireInt());
            break;

        case IControlleurPrincipal::Fonction::SetDebug:
            controlleur.setDebug();
            break;
            
        case IControlleurPrincipal::Fonction::StopDebug:
            controlleur.stopDebug();
            break;

		default:
			Serial.write(IControlleurPrincipal::Fonction::Erreur);
			ecrireInt(IControlleurPrincipal::TypeErreur::FonctionInconnue);
			Serial.flush();
        }
    }
}