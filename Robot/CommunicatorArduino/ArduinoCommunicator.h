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

    virtual void ecrire(int message);
    virtual int lire();
    static void *appliquerFonctionLecture(void* s);

public: 
    ~ArduinoCommunicator();

    /// \overload
    virtual bool avancePendantXDixiemeSec(int dixiemeSec);

    /// \overload
    virtual bool reculePendantXDixiemeSec(int dixiemeSec);

    /// \overload
    virtual bool stop();

    /// \overload
    virtual bool tourneAuDegresX(int degres);

    /// \overload
    virtual bool tourneGauche(int degres);

    /// \overload
    virtual bool tourneDroite(int degres);

    /// \overload
    virtual int obtenirOrientation();

    /// \overload
    virtual void setDebug();

    /// \overload
    virtual void stopDebug();

    /// \overload
    virtual void resetErreur();

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
