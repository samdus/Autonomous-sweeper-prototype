#include "ArduinoCommunicator.h"

bool ArduinoCommunicator::init()
{
    try 
    {
        _serial = new serial::Serial(ARDUINO_COMUNICATOR_PORT, ARDUINO_COMUNICATOR_BAUD, serial::Timeout::simpleTimeout(1000));
        return true;
    }
    catch (serial::IOException e)
    {
        return false;
    }
}

ArduinoCommunicator::~ArduinoCommunicator()
{
	stopFonctionLecture();

    if(_serial != NULL)
        delete _serial;
}

void ArduinoCommunicator::ecrire(uint8_t message)
{
    uint8_t *donnees = new uint8_t[1];
    donnees[0] = message;

    _serial->write(donnees, 1);

    delete donnees;
}

void ArduinoCommunicator::ecrireInt(int message)
{
    uint8_t *donnees = new uint8_t[2];

    donnees[0] = (message << 8) >> 8;
    donnees[1] = message >> 8;

    _serial->write(donnees, 2);

    delete donnees;
}

uint8_t ArduinoCommunicator::lire()
{
    uint8_t retour;
    uint8_t *lecture = new uint8_t[1];

    while (!_stopFonctionLectureFlag && !_serial->available());

    _serial->read(lecture, 1);
    retour = lecture[0];

    delete lecture;
    return retour;
}

int ArduinoCommunicator::lireInt()
{
    int retour;
    uint8_t *lecture = new uint8_t[2];

    while (!_stopFonctionLectureFlag && _serial->available() < 2);

    _serial->read(lecture, 2);
    retour = (lecture[1] << 8) | lecture[0];
	
    delete lecture;
    return retour;
}

int ArduinoCommunicator::getRetour()
{
	int retour;
	pthread_mutex_lock(&_mutexLecture);

	while(_intDisponibles.empty())
		pthread_cond_wait(&_conditionLecture, &_mutexLecture);

	retour = _intDisponibles.front();
	_intDisponibles.pop();
	
	pthread_mutex_unlock(&_mutexLecture);
	return retour;
}

void *ArduinoCommunicator::appliquerFonctionLecture(void* s)
{
    ArduinoCommunicator* self = (ArduinoCommunicator*)s;

    while (!self->_stopFonctionLectureFlag) 
	{
        int lecture[4] = { 0 };

        lecture[0] = self->lire();

		switch (lecture[0])
		{
		case Fonction::Erreur:
		case Fonction::InfoDistanceObjet:
		case Fonction::InfoOrientation:
			lecture[1] = self->lireInt();
			self->_callbackFonctionLecture(lecture);
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
		}
    }
    return s;
}

 bool ArduinoCommunicator::avancePendantXDixiemeSec(int dixiemeSec)
{
    ecrire(Fonction::Avance);
    ecrireInt(dixiemeSec);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::reculePendantXDixiemeSec(int dixiemeSec)
{
    ecrire(Fonction::Recule);
    ecrireInt(dixiemeSec);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::tourneAuDegresX(int degres)
{
    ecrire(Fonction::Tourne);
    ecrireInt(degres);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::tourneGauche(int degres)
{
    ecrire(Fonction::Gauche);
    ecrireInt(degres);

    return getRetour() == 1;
}

 bool ArduinoCommunicator::tourneDroite(int degres)
{
    ecrire(Fonction::Droite);
    ecrireInt(degres);

    return getRetour() == 1;
}

 int ArduinoCommunicator::obtenirOrientation()
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

void ArduinoCommunicator::setFonctionLecture(void fonction(int[4]))
{
    _callbackFonctionLecture = fonction;
    _stopFonctionLectureFlag = false;
	_threadEnFonction = pthread_create(&_thread, NULL, appliquerFonctionLecture, this) == 0;
}

void ArduinoCommunicator::stopFonctionLecture()
{
	if (_threadEnFonction)
	{
		void* status;
		_stopFonctionLectureFlag = true;
		pthread_kill(_thread, 9);
		pthread_join(_thread, &status);
	}
}