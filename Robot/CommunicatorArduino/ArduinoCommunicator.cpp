#include "ArduinoCommunicator.h"

ArduinoCommunicator::~ArduinoCommunicator()
{
	if (_serial != NULL)
	{
		stopFonctionLecture();
		delete _serial;
	}
}

bool ArduinoCommunicator::init(void fonction(int16_t[4]))
{
	try
	{
		_serial = new serial::Serial(ARDUINO_COMUNICATOR_PORT, ARDUINO_COMUNICATOR_BAUD, serial::Timeout::simpleTimeout(1000));
		_serial->setDTR(false);

		_threadEnFonction = false;
		_stopFonctionLectureFlag = false;
		_finInit = false;

		setFonctionLecture(fonction);
		
		ecrire(Fonction::FinInit);

		pthread_mutex_lock(&_mutexLecture);
		while (!_finInit && !_stopFonctionLectureFlag)
			pthread_cond_wait(&_conditionLecture, &_mutexLecture);
		pthread_mutex_unlock(&_mutexLecture);

		return _serial->isOpen() && _threadEnFonction;
	}
	catch (serial::IOException e)
	{
		return false;
	}
}

void ArduinoCommunicator::setFonctionLecture(void fonction(int16_t[4]))
{
	_callbackFonctionLecture = fonction;
	_stopFonctionLectureFlag = false;
	_threadEnFonction = pthread_create(&_thread, NULL, appliquerFonctionLecture, this) == 0;
}

void ArduinoCommunicator::ecrire(uint8_t message)
{
    uint8_t *donnees = new uint8_t[1];
    donnees[0] = message;

	try
	{
		_serial->write(donnees, 1);
	}
	catch (serial::IOException ex)
	{
		int16_t erreur[4] = { Fonction::Erreur, TypeErreur::IO, 0, 0 };
		_callbackFonctionLecture(erreur);

		stopFonctionLecture();
	}

    delete donnees;
}

void ArduinoCommunicator::ecrireInt(int16_t message)
{
    uint8_t *donnees = new uint8_t[2];

    donnees[0] = (message << 8) >> 8;
    donnees[1] = message >> 8;
	try
	{
		_serial->write(donnees, 2);
	}
	catch (serial::IOException ex)
	{
		int16_t erreur[4] = { Fonction::Erreur, TypeErreur::IO, 0, 0 };
		_callbackFonctionLecture(erreur);

		stopFonctionLecture();
	}
    delete donnees;
}

uint8_t ArduinoCommunicator::lire()
{
    uint8_t retour = 0;
    uint8_t *lecture = new uint8_t[1];

	try
	{
		while (!_stopFonctionLectureFlag && !_serial->available());

		_serial->read(lecture, 1);
		retour = lecture[0];
	}
	catch (serial::IOException ex) 
	{
		int16_t erreur[4] = {Fonction::Erreur, TypeErreur::IO, 0, 0};
		_callbackFonctionLecture(erreur);

		stopFonctionLecture();
	}

    delete lecture;
    return retour;
}

int16_t ArduinoCommunicator::lireInt()
{
	int16_t retour;
    uint8_t *lecture = new uint8_t[2];

	try
	{
		while (!_stopFonctionLectureFlag && _serial->available() < 2);

		_serial->read(lecture, 2);
		retour = (lecture[1] << 8) | lecture[0];
	}
	catch (serial::IOException ex)
	{
		int16_t erreur[4] = { Fonction::Erreur, TypeErreur::IO, 0, 0 };
		_callbackFonctionLecture(erreur);

		stopFonctionLecture();
	}

    delete lecture;
    return retour;
}

int16_t ArduinoCommunicator::getRetour()
{
	int16_t retour = -1;
	pthread_mutex_lock(&_mutexLecture);

	while(_intDisponibles.empty() && !_stopFonctionLectureFlag)
		pthread_cond_wait(&_conditionLecture, &_mutexLecture);

	if (!_intDisponibles.empty())
	{
		retour = _intDisponibles.front();
		_intDisponibles.pop();
	}
	
	pthread_mutex_unlock(&_mutexLecture);
	return retour;
}

void *ArduinoCommunicator::appliquerFonctionLecture(void* s)
{
    ArduinoCommunicator* self = (ArduinoCommunicator*)s;

    while (!self->_stopFonctionLectureFlag) 
	{
        int16_t lecture[4] = { 0 };

        lecture[0] = self->lire();

		if (!self->_stopFonctionLectureFlag) {
			switch (lecture[0])
			{
			case Fonction::Erreur:
			case Fonction::InfoDistanceObjet:
			case Fonction::InfoOrientation:
				lecture[1] = self->lireInt();

				if (lecture[1] == TypeErreur::ErreurInitialisation)
				{
					self->stopFonctionLecture();

					pthread_mutex_lock(&self->_mutexLecture);
					self->_finInit = true;
					pthread_cond_signal(&self->_conditionLecture);
					pthread_mutex_unlock(&self->_mutexLecture);
				}
				
				self->_callbackFonctionLecture(lecture);				
				break;

			case Fonction::FinInit:
				pthread_mutex_lock(&self->_mutexLecture);
				self->_finInit = true;
				pthread_cond_signal(&self->_conditionLecture);
				pthread_mutex_unlock(&self->_mutexLecture);
				break;

			case Fonction::InfoVitesseMoteur:
				lecture[1] = self->lireInt();
				lecture[2] = self->lireInt();
				self->_callbackFonctionLecture(lecture);
				break;

			case Fonction::RetourBool:
				pthread_mutex_lock(&self->_mutexLecture);

				self->_intDisponibles.push(self->lire());

				pthread_cond_signal(&self->_conditionLecture);
				pthread_mutex_unlock(&self->_mutexLecture);
				break;

			case Fonction::RetourInt:
				pthread_mutex_lock(&self->_mutexLecture);

				self->_intDisponibles.push(self->lireInt());

				pthread_cond_signal(&self->_conditionLecture);
				pthread_mutex_unlock(&self->_mutexLecture);
				break;

			case Fonction::DirectionChoisie:
				lecture[1] = self->lireInt();
				self->_callbackFonctionLecture(lecture);
				break;
			default:
				lecture[2] = lecture[0];
				lecture[0] = Fonction::Erreur;
				lecture[1] = TypeErreur::EntreeInconnue;

				self->_callbackFonctionLecture(lecture);
			}
		}
    }
    return s;
}

 bool ArduinoCommunicator::avancePendantXDixiemeSec(int16_t dixiemeSec)
{
    ecrire(Fonction::Avance);
    ecrireInt(dixiemeSec);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::reculePendantXDixiemeSec(int16_t dixiemeSec)
{
    ecrire(Fonction::Recule);
    ecrireInt(dixiemeSec);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::tourneAuDegresX(int16_t degres)
{
    ecrire(Fonction::Tourne);
    ecrireInt(degres);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::tourneGauche(int16_t degres)
{
    ecrire(Fonction::Gauche);
    ecrireInt(degres);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::tourneDroite(int16_t degres)
{
    ecrire(Fonction::Droite);
    ecrireInt(degres);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::tourneGauchePendant(int16_t dixiemeSec)
 {
	 ecrire(Fonction::GauchePendant);
	 ecrireInt(dixiemeSec);

	 return getRetour() == 1;
 }

 bool ArduinoCommunicator::tourneDroitePendant(int16_t dixiemeSec)
 {
	 ecrire(Fonction::DroitePendant);
	 ecrireInt(dixiemeSec);

	 return getRetour() == 1;
 }

int16_t ArduinoCommunicator::obtenirOrientation()
{
    ecrire(Fonction::Orientation);
	return getRetour();
}

 void ArduinoCommunicator::setDebug()
{
    ecrire(Fonction::SetDebug);
}
 void ArduinoCommunicator::stopDebug()
{
    ecrire(Fonction::StopDebug);
}
 void ArduinoCommunicator::resetErreur()
{
    ecrire(Fonction::ResetErreur);
}

bool ArduinoCommunicator::isLectureEnFonction()
{
	bool enFonction;

	pthread_mutex_lock(&_mutexFermerThread);
	enFonction = _threadEnFonction;
	pthread_mutex_unlock(&_mutexFermerThread);

	return enFonction;
}

void ArduinoCommunicator::stopFonctionLecture()
{
	bool enFonction;

	pthread_mutex_lock(&_mutexFermerThread);
	enFonction = _threadEnFonction;
	pthread_mutex_unlock(&_mutexFermerThread);
	
	if (enFonction)
	{
		void* status;
		_stopFonctionLectureFlag = true;

		pthread_kill(_thread, 9);
		pthread_join(_thread, &status);

		pthread_mutex_lock(&_mutexFermerThread);
		_threadEnFonction = false;
		pthread_mutex_unlock(&_mutexFermerThread);

		pthread_mutex_lock(&_mutexLecture);
		pthread_cond_signal(&_conditionLecture);
		pthread_mutex_unlock(&_mutexLecture);
	}
}