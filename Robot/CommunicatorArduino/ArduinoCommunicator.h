#ifndef ARDUINO_COMUNICATOR_H
#define ARDUINO_COMUNICATOR_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define HAVE_STRUCT_TIMESPEC
#endif

#include <pthread.h>
#include <queue>
#include <stdio.h>

#include "../Serial_wjwwood/include/serial/serial.h"
#include "../Software/IControlleurPrincipal.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define ARDUINO_COMUNICATOR_PORT "COM6"
#else
#define ARDUINO_COMUNICATOR_PORT "/dev/ttyACM0"
#endif

#define ARDUINO_COMUNICATOR_BAUD 9600

#include <iostream>

class ArduinoCommunicator : public IControlleurPrincipal
{
private:
    serial::Serial *_serial = NULL;
    pthread_t _thread;
	bool _threadEnFonction = false;
    bool _stopFonctionLectureFlag;
	pthread_mutex_t _mutexLecture = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t _mutexFermerThread = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t _conditionLecture = PTHREAD_COND_INITIALIZER;
	std::queue<int> _intDisponibles; 

    void(*_callbackFonctionLecture)(int[4]);

    virtual void ecrire(uint8_t message);
    virtual void ecrireInt(int message);
	virtual uint8_t lire();
    virtual int lireInt();

	virtual int getRetour();

	static void *appliquerFonctionLecture(void* s);

public: 
    ~ArduinoCommunicator();

    // \brief Permet d'initialiser le port s�rie
    bool init();

    /// \overload
	 virtual bool avancePendantXDixiemeSec(int dixiemeSec);

    /// \overload
	 virtual bool reculePendantXDixiemeSec(int dixiemeSec);
	 
    /// \overload
	 virtual bool tourneAuDegresX(int degres);

    /// \overload
	 virtual bool tourneGauche(int degres);

    /// \overload
	 virtual bool tourneDroite(int degres);

	 /// \overload
	 virtual bool tourneGauchePendant(int dixiemeSec);

	 /// \overload
	 virtual bool tourneDroitePendant(int dixiemeSec);

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

	/// \brief indique si la lecture est encore en fonction
	bool lectureEnFonction();

    /// \brief Arr�te le processus de lecture
    void stopFonctionLecture();
};
#endif // !ARDUINO_COMUNICATOR_H