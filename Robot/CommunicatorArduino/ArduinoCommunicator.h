#ifndef ARDUINO_COMUNICATOR_H
#define ARDUINO_COMUNICATOR_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define HAVE_STRUCT_TIMESPEC
#else
#include <signal.h>
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
	std::queue<int16_t> _intDisponibles;

    void(*_callbackFonctionLecture)(int16_t[4]);

    virtual void ecrire(uint8_t message);
    virtual void ecrireInt(int16_t message);
	virtual uint8_t lire();
    virtual int16_t lireInt();

	virtual int16_t getRetour();

	static void *appliquerFonctionLecture(void* s);

public: 
    ~ArduinoCommunicator();

    // \brief Permet d'initialiser le port s�rie
    bool init();

    /// \overload
	 virtual bool avancePendantXDixiemeSec(int16_t dixiemeSec);

    /// \overload
	 virtual bool reculePendantXDixiemeSec(int16_t dixiemeSec);
	 
    /// \overload
	 virtual bool tourneAuDegresX(int16_t degres);

    /// \overload
	 virtual bool tourneGauche(int16_t degres);

    /// \overload
	 virtual bool tourneDroite(int16_t degres);

	 /// \overload
	 virtual bool tourneGauchePendant(int16_t dixiemeSec);

	 /// \overload
	 virtual bool tourneDroitePendant(int16_t dixiemeSec);

    /// \overload
	 virtual int16_t obtenirOrientation();

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
    void setFonctionLecture(void fonction(int16_t[4]));

	/// \brief indique si la lecture est encore en fonction
	bool lectureEnFonction();

    /// \brief Arr�te le processus de lecture
    void stopFonctionLecture();
};
#endif // !ARDUINO_COMUNICATOR_H