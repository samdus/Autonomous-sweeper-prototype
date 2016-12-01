#include "ControlleurPrincipal.h"

ControlleurPrincipal::ControlleurPrincipal(StepperDriver *moteurGauche,  StepperDriver *moteurDroit,  SonarDriver *sonarDriver,  CompassDriver *compassDriver, int** retourFonction, void(**executionASync)(ControlleurPrincipal&))
{
	_executionASync = executionASync;
	_retourDeFonction = retourFonction;
    _moteurGauche = moteurGauche;
    _moteurDroit = moteurDroit;
    _sonarDriver = sonarDriver;
    _compassDriver = compassDriver;
    _modeDebug = false;
    _erreur = false;
    _avance = false;
    _recule = false;
}

bool ControlleurPrincipal::init(void(*transmettreDonnee)(int, bool), void(*resetTemps)(), unsigned long(*obtenirTemps)(), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4])
{
    _transmettreDonnee = transmettreDonnee;
	_resetTemps = resetTemps;
	_obtenirTemps = obtenirTemps;

    _moteurGauche->init(pinsMoteurGauche[0], pinsMoteurGauche[2], pinsMoteurGauche[1], pinsMoteurGauche[3]);
    _moteurDroit->init(pinsMoteurDroit[0], pinsMoteurDroit[2], pinsMoteurDroit[1], pinsMoteurDroit[3]);
	
	if (!_compassDriver->init())
		return false;

    _moteurGauche->avant();
    _moteurDroit->avant();
    
    _derniereOrientation = _compassDriver->getOrientation();
	return true;
}

void ControlleurPrincipal::stepMoteur()
{
    _moteurGauche->step();
    _moteurDroit->step();
}

void ControlleurPrincipal::calibrerMoteur()
{
	float orientation = _compassDriver->getOrientation();
	if (_avance || _recule)
	{		
		float difference = _derniereOrientation - orientation;

		unsigned short vitesseGauche = _moteurGauche->getVitesse();
		unsigned short vitesseDroite = _moteurDroit->getVitesse();

		if ((difference > 1.5 && _avance) ||
			(difference < -1.5 && _recule))
		{
			if (vitesseDroite == 8)
			{
				_moteurGauche->setVitesse(vitesseGauche - 1);
			}
			else
			{
				_moteurDroit->setVitesse(vitesseDroite + 1);
			}
		}
		else if ((difference >  1.5 && _recule) ||
			    (difference < -1.5 && _avance))
		{
			if (vitesseGauche == 8)
			{
				_moteurDroit->setVitesse(vitesseDroite - 1);
			}
			else
			{
				_moteurGauche->setVitesse(vitesseGauche + 1);
			}
		}
	}

	if (_modeDebug && (_itDebug++ % 10) == 0)
	{
		_transmettreDonnee(IControlleurPrincipal::Fonction::InfoOrientation, false);
		_transmettreDonnee((int)orientation, true);

		_transmettreDonnee(IControlleurPrincipal::Fonction::InfoVitesseMoteur, false);
		_transmettreDonnee(STEPPER_GAUCHE, true);
		_transmettreDonnee(_moteurGauche->getVitesse(), true);

		_transmettreDonnee(IControlleurPrincipal::Fonction::InfoVitesseMoteur, false);
		_transmettreDonnee(STEPPER_DROIT, true);
		_transmettreDonnee(_moteurDroit->getVitesse(), true);
	}
}

void ControlleurPrincipal::verifierObstacle()
{
	if (_avance)
	{
		_sonarDriver->updateDist();

		if (_sonarDriver->isObstacle())
		{
			_erreur = true;

			_moteurGauche->stop();
			_moteurDroit->stop();

			_transmettreDonnee(IControlleurPrincipal::Fonction::Erreur, false);
			_transmettreDonnee(IControlleurPrincipal::TypeErreur::Obstacle, true);
		}
	}

	if (_modeDebug && _itDebug % 10 == 0)
	{
		_transmettreDonnee(IControlleurPrincipal::Fonction::InfoDistanceObjet, false);
		_transmettreDonnee(_sonarDriver->getDist(), true);
	}
}

bool ControlleurPrincipal::isErreur()  const
{
	return _erreur;
}

bool ControlleurPrincipal::isDebug() const
{
	return _modeDebug;
}

void ControlleurPrincipal::avancePendantXDixiemeSec(int16_t dixiemeSec)
{
    _moteurGauche->avant();
    _moteurDroit->avant();
    
    _moteurGauche->avance();
    _moteurDroit->avance();

    _avance = true;

	_derniereOrientation = _compassDriver->getOrientation();
	_tempMouvementLineaire = dixiemeSec * 100;
	_resetTemps();
	(*_executionASync) = verifierTempsMouvementLineaire;
}

void ControlleurPrincipal::reculePendantXDixiemeSec(int16_t dixiemeSec)
{
    _moteurGauche->derriere();
    _moteurDroit->derriere();
    
    _moteurGauche->avance();
    _moteurDroit->avance();

    _recule = true;

	_derniereOrientation = _compassDriver->getOrientation();
	_tempMouvementLineaire = dixiemeSec * 100;
	_resetTemps();
	(*_executionASync) = verifierTempsMouvementLineaire;
}

bool ControlleurPrincipal::stop()
{
    _avance = false;
    _recule = false;

    _moteurGauche->stop();
    _moteurDroit->stop();

    return !_erreur;
}

void ControlleurPrincipal::tourneAuDegresX(int16_t objectif)
{
	float diff;
	_derniereOrientation = _compassDriver->getOrientation();
	diff = objectif - _derniereOrientation;
	
	if (fabs(diff) <= CTRL_PRINC_DIFF_ANGLE_ACCEPTE || fabs(diff) >= 360 - CTRL_PRINC_DIFF_ANGLE_ACCEPTE)
	{
		/*_transmettreDonnee(IControlleurPrincipal::Fonction::DirectionChoisie, false);
		_transmettreDonnee(0, true);*/
		return;
	}
	else if ((diff < 0 && diff > -180) || diff > 180)
	{
		/*_transmettreDonnee(IControlleurPrincipal::Fonction::DirectionChoisie, false);
		_transmettreDonnee(1, true);*/

		_moteurGauche->gauche();
		_moteurDroit->gauche();
	}
	else
	{
		/*_transmettreDonnee(IControlleurPrincipal::Fonction::DirectionChoisie, false);
		_transmettreDonnee(2, true);*/

		_moteurGauche->droite();
		_moteurDroit->droite();
	}

	_moteurGauche->avance();
	_moteurDroit->avance();

	_destinationRotation = objectif;
	(*_executionASync) = verifierDestinationRotation;
}

void ControlleurPrincipal::verifierDestinationRotation(ControlleurPrincipal &self)
{
	float diff;

	self._derniereOrientation = self._compassDriver->getOrientation();
	diff = self._destinationRotation - self._derniereOrientation;
	
	if (fabs(diff) <= CTRL_PRINC_DIFF_ANGLE_ACCEPTE)
	{
		self._moteurGauche->stop();
		self._moteurDroit->stop();

		(*self._retourDeFonction) = new int(!self._erreur);
		(*self._executionASync) = NULL;
	}
}

void ControlleurPrincipal::verifierTempsMouvementLineaire(ControlleurPrincipal &self)
{
	if (self._obtenirTemps() >= self._tempMouvementLineaire)
	{
		(*self._retourDeFonction) = new int(self.stop());
		(*self._executionASync) = NULL;
	}
}

void ControlleurPrincipal::tourneGauche(int16_t degres)
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, true);
    return tourneAuDegresX((int) angleFinal);
}

void ControlleurPrincipal::tourneDroite(int16_t degres)
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, false);
    return tourneAuDegresX((int)angleFinal);
}

void ControlleurPrincipal::tourneGauchePendant(int16_t dixiemeSec)
{
	_moteurGauche->gauche();
	_moteurDroit->gauche();

	_moteurGauche->avance();
	_moteurDroit->avance();

	_tempMouvementLineaire = dixiemeSec * 100;
	_resetTemps();
	(*_executionASync) = verifierTempsMouvementLineaire;
}

void ControlleurPrincipal::tourneDroitePendant(int16_t dixiemeSec)
{
	_moteurGauche->droite();
	_moteurDroit->droite();

	_moteurGauche->avance();
	_moteurDroit->avance();

	_tempMouvementLineaire = dixiemeSec * 100;
	_resetTemps();
	(*_executionASync) = verifierTempsMouvementLineaire;
}

void ControlleurPrincipal::obtenirOrientation()
{
	(*_retourDeFonction) = new int(round(_compassDriver->getOrientation()));
}

void ControlleurPrincipal::obtenirDistanceDevant()
{
	_sonarDriver->updateDist();
	(*_retourDeFonction) = new int(round(_sonarDriver->getDist()));
}

void ControlleurPrincipal::obtenirObstacle()
{
	_sonarDriver->updateDist();
	(*_retourDeFonction) = new int(_sonarDriver->isObstacle());
}

 void ControlleurPrincipal::setDebug()
{
    _modeDebug = true;
}

 void ControlleurPrincipal::stopDebug()
{
    _modeDebug = false;
}

 void ControlleurPrincipal::resetErreur()
{
    _erreur = false;
    _avance = false;
    _recule = false;
}

float ControlleurPrincipal::getAngleResultant(float depart, float angle, bool gauche)
{
    float resultat;

	if (angle > 360)
		angle = 360;
	else if (angle < 0)
		angle = 0;

    if (gauche)
    {
		resultat = (float) fmod(depart + angle, 360);
    }
    else
    {
        resultat = depart - angle;

        if (resultat < 0)
            resultat = 360 + resultat;
    }

    return resultat;
}
