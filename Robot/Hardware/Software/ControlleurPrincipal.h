#ifndef CONTROLLEUR_PRINCIPAL_H
#define CONTROLLEUR_PRINCIPAL_H

#include "IControlleurPrincipal.h"

#include <math.h>
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

    void(*_transmettreDonnee)(int, bool);
    void(*_attendre)(unsigned long);

	float getAngleResultant(float depart, float angle, bool gauche);

public:
    ControlleurPrincipal(StepperDriver*,  StepperDriver*,  SonarDriver*,  CompassDriver*);

	 bool init(void(*)(int, bool), void(*)(unsigned long), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4]);

	 void stepMoteur();
	 void calibrerMoteur();
	 void verifierObstacle();

	 bool isErreur() const;
	 bool isDebug() const;

	 virtual bool stop();

    /// \overload
	 virtual bool avancePendantXDixiemeSec(int16_t dixiemeSec);

    /// \overload
	 virtual bool reculePendantXDixiemeSec(int16_t dixiemeSec);

    /// \overload
	 virtual bool tourneAuDegresX(int16_t degres);

    /// \overload
	 virtual bool tourneGauche(int16_t degres);

    /// \overload
	 virtual bool tourneDroite(int16_t degres);

	 /// \overload
	 virtual bool tourneGauchePendant(int16_t dixiemeSec);

	 /// \overload
	 virtual bool tourneDroitePendant(int16_t dixiemeSec);

    /// \overload
	 virtual int16_t obtenirOrientation();

    /// \overload
	 virtual void setDebug();

    /// \overload
	 virtual void stopDebug();

    /// \overload
	 virtual void resetErreur();
};


#endif // !CONTROLLEUR_PRINCIPAL_H