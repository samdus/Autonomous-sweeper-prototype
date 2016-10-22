#ifndef CONTROLLEUR_PRINCIPAL_H
#define CONTROLLEUR_PRINCIPAL_H

#include "IControlleurPrincipal.h"

#include "ControlleurPrincipal.h"
#include "CompassDriver.h"
#include "SonarDriver.h"
#include "StepperDriver.h"

#ifndef NULL
    #define NULL 0
#endif

class ControlleurPrincipal : public IControlleurPrincipal
{
private:
	volatile StepperDriver *_moteurGauche,
						   *_moteurDroit;
	volatile SonarDriver   *_sonarDriver;
	volatile CompassDriver *_compassDriver;

	volatile bool _modeDebug;
	volatile bool _erreur;
	volatile bool _avance;
	volatile bool _recule;
	volatile float _derniereOrientation;

    void(*_transmettreDonnee)(int);
    void(*_attendre)(unsigned long);

	volatile float getAngleResultant(float depart, float angle, bool gauche)volatile;

public:
    ControlleurPrincipal(volatile StepperDriver*, volatile StepperDriver*, volatile SonarDriver*, volatile CompassDriver*);

	volatile void init(void(*)(int), void(*)(unsigned long), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4])volatile;

	volatile void stepMoteur()volatile;
	volatile void calibrerMoteur()volatile;
	volatile void verifierObstacle()volatile;

	volatile bool isErreur()volatile const;
	volatile bool isDebug()volatile const;

    /// \overload
	volatile virtual bool avancePendantXDixiemeSec(int dixiemeSec)volatile;

    /// \overload
	volatile virtual bool reculePendantXDixiemeSec(int dixiemeSec)volatile;

    /// \overload
	volatile virtual bool stop()volatile;

    /// \overload
	volatile virtual bool tourneAuDegresX(int degres)volatile;

    /// \overload
	volatile virtual bool tourneGauche(int degres)volatile;

    /// \overload
	volatile virtual bool tourneDroite(int degres)volatile;

    /// \overload
	volatile virtual int obtenirOrientation()volatile;

    /// \overload
	volatile virtual void setDebug()volatile;

    /// \overload
	volatile virtual void stopDebug()volatile;

    /// \overload
	volatile virtual void resetErreur()volatile;
};


#endif // !CONTROLLEUR_PRINCIPAL_H