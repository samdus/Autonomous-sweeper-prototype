#ifndef CONTROLLEUR_PRINCIPAL_H
#define CONTROLLEUR_PRINCIPAL_H

#include <math.h>

#include "IControlleurPrincipal.h"
#include "CompassDriver.h"
#include "SonarDriver.h"
#include "StepperDriver.h"

#ifndef NULL
    #define NULL 0
#endif

class ControlleurPrincipal
{
protected:
	StepperDriver *_moteurGauche,
				  *_moteurDroit;
	SonarDriver *_sonarDriver;
	CompassDriver *_compassDriver;

	void(**_executionASync)(ControlleurPrincipal&);
	int **_retourDeFonction;

	bool _modeDebug;
	bool _erreur;
	bool _avance;
	bool _recule;
	float _derniereOrientation;
	float _destinationRotation;
	unsigned long _tempMouvementLineaire;

    void(*_transmettreDonnee)(int, bool);

	void(*_resetTemps)();
	unsigned long(*_obtenirTemps)();

	virtual float getAngleResultant(float depart, float angle, bool gauche);
	virtual void calibrerMoteur();
	virtual void verifierObstacle();
	virtual void transmettreDonneesDebug();
	
	static void verifierDestinationRotation(ControlleurPrincipal&);
	static void verifierTempsMouvementLineaire(ControlleurPrincipal&);

	virtual bool stop();

public:
	ControlleurPrincipal(StepperDriver*,  StepperDriver*,  SonarDriver*,  CompassDriver*, int**, void(**executionASync)(ControlleurPrincipal&));
	
	bool init(void(*)(int, bool), void (*)(), unsigned long(*)(), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4]);

	void stepMoteur();
	void mettreAJourCapteurs();

	bool isErreur() const;
	bool isDebug() const;

	virtual void avancePendantXDixiemeSec(int16_t dixiemeSec);
	virtual void reculePendantXDixiemeSec(int16_t dixiemeSec);
	virtual void tourneAuDegresX(int16_t degres);
	virtual void tourneGauche(int16_t degres);
	virtual void tourneDroite(int16_t degres);
	virtual void obtenirOrientation();
	virtual void obtenirDistanceDevant();
	virtual void obtenirObstacle();
	virtual void setDebug();
	virtual void stopDebug();
	virtual void resetErreur();
};

#endif // !CONTROLLEUR_PRINCIPAL_H