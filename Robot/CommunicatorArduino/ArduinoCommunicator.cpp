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
	return lire(false);
}

uint8_t ArduinoCommunicator::lire(bool verifierFlag)
{
    uint8_t retour;
    uint8_t *lecture = new uint8_t[1];

    while ((!verifierFlag || !_stopFonctionLectureFlag) && !_serial->available());

    _serial->read(lecture, 1);
    retour = lecture[0];

    delete lecture;
    return retour;
}

int ArduinoCommunicator::lireInt()
{
    return lireInt(false);
}

int ArduinoCommunicator::lireInt(bool verifierFlag)
{
    int retour;
    uint8_t *lecture = new uint8_t[2];

    while ((!verifierFlag || !_stopFonctionLectureFlag) && _serial->available() < 2);

    _serial->read(lecture, 2);
    retour = (lecture[1] << 8) | lecture[0];

    delete lecture;
    return retour;
}

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
            lecture[1] = self->lireInt(true);
        }

        self->_callback(lecture);
    }
    return s;
};

 bool ArduinoCommunicator::avancePendantXDixiemeSec(int dixiemeSec)
{
    ecrire(Fonction::Avance);
    ecrireInt(dixiemeSec);

    return lire() == 1;
}

 bool ArduinoCommunicator::reculePendantXDixiemeSec(int dixiemeSec)
{
    ecrire(Fonction::Recule);
    ecrireInt(dixiemeSec);

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
    ecrireInt(degres);

    return lire() == 1;
}

 bool ArduinoCommunicator::tourneGauche(int degres)
{
    ecrire(Fonction::Gauche);
    ecrireInt(degres);

    return lire() == 1;
}

 bool ArduinoCommunicator::tourneDroite(int degres)
{
    ecrire(Fonction::Droite);
    ecrireInt(degres);

    return lire() == 1;
}

 int ArduinoCommunicator::obtenirOrientation()
{
    ecrire(Fonction::Orientation);
	return lireInt();
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