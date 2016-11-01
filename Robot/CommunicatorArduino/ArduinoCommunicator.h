#ifndef ARDUINO_COMUNICATOR_H
#define ARDUINO_COMUNICATOR_H


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	#include <windows.h>
	#define ARDUINO_COMUNICATOR_PORT "COM6"
	#define HAVE_STRUCT_TIMESPEC
#else
	#include <signal.h>
	#include <unistd.h>
	#define ARDUINO_COMUNICATOR_PORT "/dev/ttyACM0"
#endif

#define ARDUINO_COMUNICATOR_BAUD 9600

#include <pthread.h>
#include <queue>
#include <stdio.h>

#include "../Serial_wjwwood/include/serial/serial.h"
#include "../Software/IControlleurPrincipal.h"

class ArduinoCommunicator : public IControlleurPrincipal
{
private:

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	void sleep(unsigned milliseconds)
	{
		Sleep(milliseconds);
	}
#else
	void sleep(unsigned milliseconds)
	{
		usleep(milliseconds * 1000); // takes microseconds
	}
#endif

    serial::Serial *_serial = NULL;
    pthread_t _thread;
	bool _finInit;
	bool _threadEnFonction;
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
	
	/// \brief Permet d'initialiser le port série et
	///        Cree un nouveau thread pour faire la lecture sur le port serie
	//         (Boucle exécutée en parallele)
	/// \param fonction La fonction appelee lorsque le périphérique recevra des donnees 
	///                 Elle possede le paramètre message int[4] qui
	///                 contiendra jusqu'à 4 messages lues sur le port série
    bool init(void fonction(int16_t[4]));

	// \brief Cree un nouveau thread pour faire la lecture sur le port serie
	//         (Boucle exécutée en parallele)
	/// \param fonction La fonction appelee lorsque le périphérique recevra des donnees 
	///                 Elle possede le paramètre message int[4] qui
	///                 contiendra jusqu'à 4 messages lues sur le port série
	void setFonctionLecture(void fonction(int16_t[4]));

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

	/// \brief indique si la lecture est encore en fonction
	bool isLectureEnFonction();

    /// \brief Arrête le processus de lecture
    void stopFonctionLecture();
};
#endif // !ARDUINO_COMUNICATOR_H