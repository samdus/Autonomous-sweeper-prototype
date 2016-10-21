#include "ArduinoCommunicator.h"

void ArduinoCommunicator::ecrire(int message)
{
    if (_fichier == NULL)
        _fichier = fopen(ARDUINO_COMUNICATOR_NOM_FICHIER, "w");
    fputc(message, _fichier);
};

int ArduinoCommunicator::lire()
{
    if (_fichier == NULL)
        _fichier = fopen(ARDUINO_COMUNICATOR_NOM_FICHIER, "w");

    int retour;
    do
    {
        retour = fgetc(_fichier);
    } while (retour <= 0 && !_stopFonctionLectureFlag);

    return retour;
};

void *ArduinoCommunicator::appliquerFonctionLecture(void* s)
{
    ArduinoCommunicator* self = (ArduinoCommunicator*)s;

    while (!self->_stopFonctionLectureFlag) {
        int lecture[4] = { 0 };

        lecture[0] = self->lire();

        if (lecture[0] == Fonction::Erreur ||
            lecture[0] == Fonction::InfoDistanceObjet ||
            lecture[0] == Fonction::InfoOrientation ||
            lecture[0] == Fonction::InfoVitesseMoteur)
        {
            lecture[1] = self->lire();
        }

        self->_callback(lecture);
    }
    return s;
};

ArduinoCommunicator::~ArduinoCommunicator()
{
    if (_fichier != NULL)
        fclose(_fichier);
}

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

bool ArduinoCommunicator::tourneGauche(int degres) {
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