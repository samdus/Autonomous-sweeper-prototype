#include "ControlleurPrincipal.h"

ControlleurPrincipal::ControlleurPrincipal(volatile StepperDriver *moteurGauche, volatile StepperDriver *moteurDroit, volatile SonarDriver *sonarDriver, volatile CompassDriver *compassDriver)
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

volatile void ControlleurPrincipal::init(void(*transmettreDonnee)(int), void(*attendre)(unsigned long), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4])volatile
{
    _transmettreDonnee = transmettreDonnee;
    _attendre = attendre;
	
    _moteurGauche->init(pinsMoteurGauche[0], pinsMoteurGauche[2], pinsMoteurGauche[1], pinsMoteurGauche[3]);
    _moteurDroit->init(pinsMoteurDroit[0], pinsMoteurDroit[2], pinsMoteurDroit[1], pinsMoteurDroit[3]);
    _compassDriver->init();

    _moteurGauche->avant();
    _moteurDroit->avant();
    
    _derniereOrientation = _compassDriver->getOrientation();
}

volatile void ControlleurPrincipal::stepMoteur()volatile
{
    _moteurGauche->step();
    _moteurDroit->step();
}

volatile void ControlleurPrincipal::calibrerMoteur()volatile
{
	if (_avance || _recule)
	{
		float orientation = _compassDriver->getOrientation();
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

		if (_modeDebug)
		{
			_transmettreDonnee(Fonction::InfoOrientation);
			_transmettreDonnee((int)orientation);

			_transmettreDonnee(Fonction::InfoVitesseMoteur);
			_transmettreDonnee(STEPPER_GAUCHE);
			_transmettreDonnee(_moteurGauche->getVitesse());

			_transmettreDonnee(Fonction::InfoVitesseMoteur);
			_transmettreDonnee(STEPPER_DROIT);
			_transmettreDonnee(_moteurDroit->getVitesse());
		}
	}
}

volatile void ControlleurPrincipal::verifierObstacle()volatile
{
	if (_avance)
	{
		_sonarDriver->updateDist();

		if (_sonarDriver->isObstacle())
		{
			_erreur = true;

			_moteurGauche->stop();
			_moteurDroit->stop();

			_transmettreDonnee(Fonction::Erreur);
			_transmettreDonnee(TypeErreur::Obstacle);
		}
		else if (_modeDebug)
		{
			_transmettreDonnee(Fonction::InfoDistanceObjet);
			_transmettreDonnee(_sonarDriver->getDist());
		}
	}
}

volatile bool ControlleurPrincipal::isErreur() volatile const
{
	return _erreur;
}

volatile bool ControlleurPrincipal::isDebug()volatile const
{
	return _modeDebug;
}

volatile bool ControlleurPrincipal::avancePendantXDixiemeSec(int dixiemeSec)volatile
{
    _moteurGauche->avant();
    _moteurDroit->avant();
    
    _moteurGauche->avance();
    _moteurDroit->avance();

    _avance = true;

    _attendre(dixiemeSec * 100);

    return stop();
}

volatile bool ControlleurPrincipal::reculePendantXDixiemeSec(int dixiemeSec)volatile
{
    _moteurGauche->derriere();
    _moteurDroit->derriere();
    
    _moteurGauche->avance();
    _moteurDroit->avance();

    _recule = true;

    _attendre(dixiemeSec * 100);

    return stop();
}

volatile bool ControlleurPrincipal::stop()volatile
{
    _avance = false;
    _recule = false;

    _moteurGauche->stop();
    _moteurDroit->stop();

    return !_erreur;
}

volatile bool ControlleurPrincipal::tourneAuDegresX(int degres)volatile
{
    if (_compassDriver->getOrientation() > degres)
    {
        _moteurGauche->droite();
        _moteurDroit->droite();

		_moteurGauche->avance();
		_moteurDroit->avance();

        while (_compassDriver->getOrientation() > degres)
        {
            _attendre(10);
        }
    }
    else
    {
        _moteurGauche->gauche();
        _moteurDroit->gauche();

		_moteurGauche->avance();
		_moteurDroit->avance();

        while (_compassDriver->getOrientation() < degres)
        {
            _attendre(10);
        }
    }

    _moteurGauche->stop();
    _moteurDroit->stop();

    return !_erreur;
}

volatile bool ControlleurPrincipal::tourneGauche(int degres)volatile
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, true);
    return tourneAuDegresX((int) angleFinal);
}

volatile bool ControlleurPrincipal::tourneDroite(int degres)volatile
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, false);
    return tourneAuDegresX((int)angleFinal);
}

volatile int ControlleurPrincipal::obtenirOrientation()volatile
{
    return (int)_compassDriver->getOrientation();
}

volatile void ControlleurPrincipal::setDebug()volatile
{
    _modeDebug = true;
}

volatile void ControlleurPrincipal::stopDebug()volatile
{
    _modeDebug = false;
}

volatile void ControlleurPrincipal::resetErreur()volatile
{
    _erreur = false;
    _avance = false;
    _recule = false;
}

volatile float ControlleurPrincipal::getAngleResultant(float depart, float angle, bool gauche)volatile
{
    float resultat;

    if (gauche)
    {
        resultat = depart + angle;

        while (resultat > 360)
            resultat -= 360;
    }
    else
    {
        resultat = depart - angle;

        while (resultat < 0)
            resultat += 360;
    }

    return resultat;
}
