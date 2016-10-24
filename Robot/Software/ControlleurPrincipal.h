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
	 StepperDriver *_moteurGauche,
				   *_moteurDroit;
	 SonarDriver   *_sonarDriver;
	 CompassDriver *_compassDriver;

	 bool _modeDebug;
	 bool _erreur;
	 bool _avance;
	 bool _recule;
	 float _derniereOrientation;

    void(*_transmettreDonnee)(int);
    void(*_attendre)(unsigned long);

	 float getAngleResultant(float depart, float angle, bool gauche);

public:
    ControlleurPrincipal( StepperDriver*,  StepperDriver*,  SonarDriver*,  CompassDriver*);

	 void init(void(*)(int), void(*)(unsigned long), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4]);

	 void stepMoteur();
	 void calibrerMoteur();
	 void verifierObstacle();

	 bool isErreur() const;
	 bool isDebug() const;

    /// \overload
	 virtual bool avancePendantXDixiemeSec(int dixiemeSec);

    /// \overload
	 virtual bool reculePendantXDixiemeSec(int dixiemeSec);

    /// \overload
	 virtual bool stop();

    /// \overload
	 virtual bool tourneAuDegresX(int degres);

    /// \overload
	 virtual bool tourneGauche(int degres);

    /// \overload
	 virtual bool tourneDroite(int degres);

    /// \overload
	 virtual int obtenirOrientation();

    /// \overload
	 virtual void setDebug();

    /// \overload
	 virtual void stopDebug();

    /// \overload
	 virtual void resetErreur();
};


#endif // !CONTROLLEUR_PRINCIPAL_H