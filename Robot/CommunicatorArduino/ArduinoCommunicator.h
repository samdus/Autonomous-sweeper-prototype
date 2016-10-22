#ifndef ARDUINO_COMUNICATOR_H
#define ARDUINO_COMUNICATOR_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define HAVE_STRUCT_TIMESPEC
#endif

#include <pthread.h>
#include <stdio.h>

#include "../Software/IControlleurPrincipal.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define ARDUINO_COMUNICATOR_NOM_FICHIER "COM6"
#else
#define ARDUINO_COMUNICATOR_NOM_FICHIER "/dev/ttyACM0"
#endif

class ArduinoCommunicator : public IControlleurPrincipal
{
private:
    FILE *_fichier = NULL;
    pthread_t _thread;
    bool _stopFonctionLectureFlag = true;

    void(*_callback)(int[4]);

	volatile virtual void ecrire(int message)volatile;
	volatile virtual int lire()volatile;
	static void *appliquerFonctionLecture(void* s);

public: 
    ~ArduinoCommunicator();

    /// \overload
	volatile virtual bool avancePendantXDixiemeSec(int dixiemeSec)volatile;

    /// \overload
	volatile virtual bool reculePendantXDixiemeSec(int dixiemeSec)volatile;

    /// \overload
	volatile virtual bool stop()volatile;

    /// \overload
	volatile virtual bool tourneAuDegresX(int degres)volatile;

    /// \overload
	volatile virtual bool tourneGauche(int degres)volatile;

    /// \overload
	volatile virtual bool tourneDroite(int degres)volatile;

    /// \overload
	volatile virtual int obtenirOrientation()volatile;

    /// \overload
	volatile virtual void setDebug()volatile;

    /// \overload
	volatile virtual void stopDebug()volatile;

    /// \overload
	volatile virtual void resetErreur()volatile;

    /// \brief Défini la fonction qui sera appelé lorsque le périphérique
    ///        recevra des données
    //         (Boucle exécutée en parallèle)
    /// \param fonction La fonction callback 
    ///                 Elle possède le paramètre message int[4] qui
    ///                 contiendra jusqu'à 4 messages lues sur le port série
    void setFonctionLecture(void fonction(int[4]));

    /// \brief Arrête le processus de lecture
    void stopFonctionLecture();
};
#endif // !ARDUINO_COMUNICATOR_H
