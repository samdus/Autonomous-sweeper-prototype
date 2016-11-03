#include "ControlleurPrincipal.h"

ControlleurPrincipal::ControlleurPrincipal( StepperDriver *moteurGauche,  StepperDriver *moteurDroit,  SonarDriver *sonarDriver,  CompassDriver *compassDriver)
{
    _moteurGauche = moteurGauche;
    _moteurDroit = moteurDroit;
    _sonarDriver = sonarDriver;
    _compassDriver = compassDriver;
    _modeDebug = false;
    _erreur = false;
    _avance = false;
    _recule = false;
}

bool ControlleurPrincipal::init(void(*transmettreDonnee)(int, bool), void(*attendre)(unsigned long), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4])
{
    _transmettreDonnee = transmettreDonnee;
    _attendre = attendre;
	
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
		_transmettreDonnee(Fonction::InfoOrientation, false);
		_transmettreDonnee((int)orientation, true);

		_transmettreDonnee(Fonction::InfoVitesseMoteur, false);
		_transmettreDonnee(STEPPER_GAUCHE, true);
		_transmettreDonnee(_moteurGauche->getVitesse(), true);

		_transmettreDonnee(Fonction::InfoVitesseMoteur, false);
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

			_transmettreDonnee(Fonction::Erreur, false);
			_transmettreDonnee(TypeErreur::Obstacle, true);
		}
	}

	if (_modeDebug && _itDebug % 10 == 0)
	{
		_transmettreDonnee(Fonction::InfoDistanceObjet, false);
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

bool ControlleurPrincipal::avancePendantXDixiemeSec(int16_t dixiemeSec)
{
    _moteurGauche->avant();
    _moteurDroit->avant();
    
    _moteurGauche->avance();
    _moteurDroit->avance();

    _avance = true;

    _attendre(dixiemeSec * 100);

    return stop();
}

bool ControlleurPrincipal::reculePendantXDixiemeSec(int16_t dixiemeSec)
{
    _moteurGauche->derriere();
    _moteurDroit->derriere();
    
    _moteurGauche->avance();
    _moteurDroit->avance();

    _recule = true;

    _attendre(dixiemeSec * 100);

    return stop();
}

bool ControlleurPrincipal::stop()
{
    _avance = false;
    _recule = false;

    _moteurGauche->stop();
    _moteurDroit->stop();

    return !_erreur;
}

bool ControlleurPrincipal::tourneAuDegresX(int16_t objectif)
{
	float diff;
	_derniereOrientation = _compassDriver->getOrientation();
	diff = objectif - _derniereOrientation;
	
	if (fabs(diff) <= CTRL_PRINC_DIFF_ANGLE_ACCEPTE || fabs(diff) >= 360 - CTRL_PRINC_DIFF_ANGLE_ACCEPTE)
	{
		_transmettreDonnee(Fonction::DirectionChoisie, false);
		_transmettreDonnee(0, true);
		return true;
	}
	else if ((diff < 0 && diff > -180) || diff > 180)
	{
		_transmettreDonnee(Fonction::DirectionChoisie, false);
		_transmettreDonnee(1, true);
		return tourneAGaucheVersXDegres(objectif);
	}
	else
	{
		_transmettreDonnee(Fonction::DirectionChoisie, false);
		_transmettreDonnee(2, true);
		return tourneADroiteVersXDegres(objectif);
	}
}

bool ControlleurPrincipal::tourneADroiteVersXDegres(int16_t objectif)
{
	float diff;// , oldDiff;

	_moteurGauche->droite();
	_moteurDroit->droite();

	_moteurGauche->avance();
	_moteurDroit->avance();


	do
	{
		//oldDiff = diff;
		_attendre(5);

		_derniereOrientation = _compassDriver->getOrientation();
		diff = objectif - _derniereOrientation;
	} while (fabs(diff) > CTRL_PRINC_DIFF_ANGLE_ACCEPTE);

	_moteurGauche->stop();
	_moteurDroit->stop();

	return !_erreur;
}

bool ControlleurPrincipal::tourneAGaucheVersXDegres(int16_t objectif)
{
	float diff;// , oldDiff;
	//int compteurInvalide = 0;

	_moteurGauche->gauche();
	_moteurDroit->gauche();

	_moteurGauche->avance();
	_moteurDroit->avance();

	//oldDiff = objectif - _derniereOrientation;

	do
	{
		_attendre(5);

		_derniereOrientation = _compassDriver->getOrientation();
		diff = objectif - _derniereOrientation;

		/*if ((diff > 0 && diff > oldDiff) || (diff < 0 && oldDiff < 0 && diff < oldDiff))
		{
			if (++compteurInvalide == 5)
				return tourneADroiteVersXDegres(objectif);
		}
		else
		{
			compteurInvalide = 0;
		}

		oldDiff = diff;*/
	} while (fabs(diff) > CTRL_PRINC_DIFF_ANGLE_ACCEPTE);

	_moteurGauche->stop();
	_moteurDroit->stop();

	return !_erreur;
}

bool ControlleurPrincipal::tourneGauche(int16_t degres)
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, true);
    return tourneAuDegresX((int) angleFinal);
}

bool ControlleurPrincipal::tourneDroite(int16_t degres)
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, false);
    return tourneAuDegresX((int)angleFinal);
}

bool ControlleurPrincipal::tourneGauchePendant(int16_t dixiemeSec)
{
	_moteurGauche->gauche();
	_moteurDroit->gauche();

	_moteurGauche->avance();
	_moteurDroit->avance();
	
	_attendre(dixiemeSec * 100);

	return stop();
}

bool ControlleurPrincipal::tourneDroitePendant(int16_t dixiemeSec)
{
	_moteurGauche->droite();
	_moteurDroit->droite();

	_moteurGauche->avance();
	_moteurDroit->avance();

	_attendre(dixiemeSec * 100);

	return stop();
}

int16_t ControlleurPrincipal::obtenirOrientation()
{
    return (int)_compassDriver->getOrientation();
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
