#include "ArduinoCommunicator.h"

ArduinoCommunicator::ArduinoCommunicator()
{
	pthread_mutex_init(&_mutexFichier, NULL);
	_fichier = fopen(ARDUINO_COMUNICATOR_NOM_FICHIER, "w");
}

ArduinoCommunicator::~ArduinoCommunicator()
{
	if (_fichier != NULL)
		fclose(_fichier);
}

void ArduinoCommunicator::ecrire(int message)
{
	pthread_mutex_lock(&_mutexFichier);
    fputc(message, _fichier);
	pthread_mutex_unlock(&_mutexFichier);
};

int ArduinoCommunicator::lire()
{
	return lire(false);
}

int ArduinoCommunicator::lire(bool verifierFlag)
{
    int retour;

    do
    {
		pthread_mutex_lock(&_mutexFichier);
        retour = fgetc(_fichier);
		pthread_mutex_unlock(&_mutexFichier);
    } while (retour <= 0 || (verifierFlag && _stopFonctionLectureFlag) || retour == 205);

    return retour;
};

void *ArduinoCommunicator::appliquerFonctionLecture(void* s)
{
    ArduinoCommunicator* self = (ArduinoCommunicator*)s;

    while (!self->_stopFonctionLectureFlag) {
        int lecture[4] = { 0 };

        lecture[0] = self->lire(true);

        if (lecture[0] == Fonction::Erreur ||
            lecture[0] == Fonction::InfoDistanceObjet ||
            lecture[0] == Fonction::InfoOrientation ||
            lecture[0] == Fonction::InfoVitesseMoteur)
        {
            lecture[1] = self->lire(true);
        }

        self->_callback(lecture);
    }
    return s;
};

 bool ArduinoCommunicator::avancePendantXDixiemeSec(int dixiemeSec)
{
    ecrire(Fonction::Avance);
    ecrire(dixiemeSec);

    return lire() == 1;
}

 bool ArduinoCommunicator::reculePendantXDixiemeSec(int dixiemeSec)
{
    ecrire(Fonction::Recule);
    ecrire(dixiemeSec);

    return lire() == 1;
}

 bool ArduinoCommunicator::stop()
{
    ecrire(Fonction::Stop);
    return lire() == 1;
}

 bool ArduinoCommunicator::tourneAuDegresX(int degres)
{
    ecrire(Fonction::Tourne);
    ecrire(degres);

    return lire() == 1;
}

 bool ArduinoCommunicator::tourneGauche(int degres)
{
    ecrire(Fonction::Gauche);
    ecrire(degres);

    return lire() == 1;
}

 bool ArduinoCommunicator::tourneDroite(int degres)
{
    ecrire(Fonction::Droite);
    ecrire(degres);

    return lire() == 1;
}

 int ArduinoCommunicator::obtenirOrientation()
{
    ecrire(Fonction::Orientation);
	return lire();
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
    _callback = fonction;
    _stopFonctionLectureFlag = false;
    pthread_create(&_thread, NULL, appliquerFonctionLecture, this);
}

void ArduinoCommunicator::stopFonctionLecture()
{
    void* status;
    _stopFonctionLectureFlag = true;
    pthread_kill(_thread, 9);
    pthread_join(_thread, &status);
}