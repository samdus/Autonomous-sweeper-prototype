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

 void ControlleurPrincipal::init(void(*transmettreDonnee)(int), void(*attendre)(unsigned long), byte pinsMoteurGauche[4], byte pinsMoteurDroit[4])
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

 void ControlleurPrincipal::stepMoteur()
{
    _moteurGauche->step();
    _moteurDroit->step();
}

 void ControlleurPrincipal::calibrerMoteur()
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

 bool ControlleurPrincipal::isErreur()  const
{
	return _erreur;
}

 bool ControlleurPrincipal::isDebug() const
{
	return _modeDebug;
}

 bool ControlleurPrincipal::avancePendantXDixiemeSec(int dixiemeSec)
{
    _moteurGauche->avant();
    _moteurDroit->avant();
    
    _moteurGauche->avance();
    _moteurDroit->avance();

    _avance = true;

    _attendre(dixiemeSec * 100);

    return stop();
}

 bool ControlleurPrincipal::reculePendantXDixiemeSec(int dixiemeSec)
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

 bool ControlleurPrincipal::tourneAuDegresX(int degres)
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

 bool ControlleurPrincipal::tourneGauche(int degres)
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, true);
    return tourneAuDegresX((int) angleFinal);
}

 bool ControlleurPrincipal::tourneDroite(int degres)
{
    float angleFinal = getAngleResultant(_compassDriver->getOrientation(), (float)degres, false);
    return tourneAuDegresX((int)angleFinal);
}

 int ControlleurPrincipal::obtenirOrientation()
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
