#ifndef ARDUINO_COMUNICATOR_H
#define ARDUINO_COMUNICATOR_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define HAVE_STRUCT_TIMESPEC
#endif

#include <pthread.h>
#include <stdio.h>

#include "../Serial_wjwwood/include/serial/serial.h"
#include "../Software/IControlleurPrincipal.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define ARDUINO_COMUNICATOR_NOM_FICHIER "COM6"
#else
#define ARDUINO_COMUNICATOR_NOM_FICHIER "/dev/ttyACM0"
#endif

#include <iostream>

class ArduinoCommunicator : public IControlleurPrincipal
{
private:
    FILE *_fichier = NULL;
    pthread_t _thread;
    bool _stopFonctionLectureFlag;
	pthread_mutex_t _mutexFichier = PTHREAD_MUTEX_INITIALIZER;

    void(*_callback)(int[4]);

	 virtual void ecrire(int message);
	 virtual int lire();
	 virtual int lire(bool);
	 static void *appliquerFonctionLecture(void* s);

public: 
	ArduinoCommunicator();
    ~ArduinoCommunicator();

	void afficherPortsDisponibles()
	{
		std::vector<serial::PortInfo> devices_found = serial::list_ports();

		std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

		while (iter != devices_found.end())
		{
			serial::PortInfo device = *iter++;

			printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
				device.hardware_id.c_str());
		}
	}

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