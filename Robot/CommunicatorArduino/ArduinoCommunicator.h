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

    /// \brief D�fini la fonction qui sera appel� lorsque le p�riph�rique
    ///        recevra des donn�es
    //         (Boucle ex�cut�e en parall�le)
    /// \param fonction La fonction callback 
    ///                 Elle poss�de le param�tre message int[4] qui
    ///                 contiendra jusqu'� 4 messages lues sur le port s�rie
    void setFonctionLecture(void fonction(int[4]));

    /// \brief Arr�te le processus de lecture
    void stopFonctionLecture();
};
#endif // !ARDUINO_COMUNICATOR_H
