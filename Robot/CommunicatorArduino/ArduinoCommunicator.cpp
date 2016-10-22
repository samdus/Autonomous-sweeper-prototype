#include "ArduinoCommunicator.h"

volatile void ArduinoCommunicator::ecrire(int message)volatile
{
    if (_fichier == NULL)
        _fichier = fopen(ARDUINO_COMUNICATOR_NOM_FICHIER, "w");
    fputc(message, _fichier);
};

volatile int ArduinoCommunicator::lire()volatile
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

volatile bool ArduinoCommunicator::avancePendantXDixiemeSec(int dixiemeSec)volatile
{
    ecrire(Fonction::Avance);
    ecrire(dixiemeSec);

    return lire() == 1;
}

volatile bool ArduinoCommunicator::reculePendantXDixiemeSec(int dixiemeSec)volatile
{
    ecrire(Fonction::Recule);
    ecrire(dixiemeSec);

    return lire() == 1;
}

volatile bool ArduinoCommunicator::stop()volatile
{
    ecrire(Fonction::Stop);
    return lire() == 1;
}

volatile bool ArduinoCommunicator::tourneAuDegresX(int degres)volatile
{
    ecrire(Fonction::Tourne);
    ecrire(degres);

    return lire() == 1;
}

volatile bool ArduinoCommunicator::tourneGauche(int degres)volatile
{
    ecrire(Fonction::Gauche);
    ecrire(degres);

    return lire() == 1;
}

volatile bool ArduinoCommunicator::tourneDroite(int degres)volatile
{
    ecrire(Fonction::Droite);
    ecrire(degres);

    return lire() == 1;
}

volatile int ArduinoCommunicator::obtenirOrientation()volatile
{
    ecrire(Fonction::Orientation);
    return lire();
}

volatile void ArduinoCommunicator::setDebug()volatile
{
    ecrire(Fonction::SetDebug);
}
volatile void ArduinoCommunicator::stopDebug()volatile
{
    ecrire(Fonction::StopDebug);
}
volatile void ArduinoCommunicator::resetErreur()volatile
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