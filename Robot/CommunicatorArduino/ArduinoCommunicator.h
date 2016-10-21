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

    virtual void ecrire(int message) 
    {
        if (_fichier == NULL)
            _fichier = fopen(ARDUINO_COMUNICATOR_NOM_FICHIER, "w");
        fputc(message, _fichier);
    };
    virtual int lire() 
    {
        if (_fichier == NULL)
            _fichier = fopen(ARDUINO_COMUNICATOR_NOM_FICHIER, "w");
        return fgetc(_fichier);
    };

    void(*_callback)(int[4]);
    static void *appliquerFonctionLecture(void* s) 
    {
        ArduinoCommunicator* self = (ArduinoCommunicator*)s;

        while (1) {
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
    };

public: 
        ~ArduinoCommunicator() 
        {
            if (_fichier != NULL)
                fclose(_fichier);
        }
        /// \overload
        virtual bool avancePendantXDixiemeSec(int dixiemeSec) 
        {
            ecrire(Fonction::Avance);
            ecrire(dixiemeSec);

            return lire() == 1;
        }

        /// \overload
        virtual bool reculePendantXDixiemeSec(int dixiemeSec) 
        {
            ecrire(Fonction::Recule);
            ecrire(dixiemeSec);

            return lire() == 1;
        }

        /// \overload
        virtual bool stop() 
        {
            ecrire(Fonction::Stop);
            return lire() == 1;
        }

        /// \overload
        virtual bool tourneAuDegresX(int degres) 
        {
            ecrire(Fonction::Tourne);
            ecrire(degres);

            return lire() == 1;
        }

        /// \overload
        virtual bool tourneGauche(int degres) {
            ecrire(Fonction::Gauche);
            ecrire(degres);

            return lire() == 1;
        }

        /// \overload
        virtual bool tourneDroite(int degres) 
        {
            ecrire(Fonction::Droite);
            ecrire(degres);

            return lire() == 1;
        }

        /// \overload
        virtual int obtenirOrientation() 
        {
            ecrire(Fonction::Orientation);
            return lire();
        }

        /// \overload
        virtual void setDebug() 
        { 
            ecrire(Fonction::SetDebug); 
        }

        /// \overload
        virtual void stopDebug() 
        { 
            ecrire(Fonction::StopDebug); 
        }

        /// \overload
        virtual void resetErreur() 
        { 
            ecrire(Fonction::ResetErreur); 
        }

        /// \brief Défini la fonction qui sera appelé lorsque le périphérique
        ///        recevra des données
        //         (Boucle exécutée en parallèle)
        /// \param fonction La fonction callback 
        ///                 Elle possède le paramètre message int[4] qui
        ///                 contiendra jusqu'à 4 messages lues sur le port série
        void setFonctionLecture(void fonction(int[4])) 
        {
            _callback = fonction;
            pthread_create(&_thread, NULL, appliquerFonctionLecture, this);
        }

        /// \brief Arrête le processus de lecture
        void stopFonctionLecture() 
        {
            void* status;
            pthread_kill(_thread, 9);
            pthread_join(_thread, &status);
        }
};
#endif // !ARDUINO_COMUNICATOR_H
