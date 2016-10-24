#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CommunicatorArduino/ArduinoCommunicator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{

    TEST_CLASS(CommunicatorArduinoTest)
    {
    public:

        TEST_METHOD(Communicator_AvancePendantXDixiemeSec)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;
            
            bool retour = communicator.avancePendantXDixiemeSec(100);

            Assert::IsTrue(communicator.itEcrire == 2);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::Avance);
            Assert::IsTrue(communicator.ecriture[1] == 100);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(Communicator_ReculePendantXDixiemeSec)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            bool retour = communicator.reculePendantXDixiemeSec(100);

            Assert::IsTrue(communicator.itEcrire == 2);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::Recule);
            Assert::IsTrue(communicator.ecriture[1] == 100);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(Communicator_Stop)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture = message; itEcrire++; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture;
            } communicator;

            bool retour = communicator.stop();

            Assert::IsTrue(communicator.itEcrire == 1);
            Assert::IsTrue(communicator.ecriture== ArduinoCommunicator::Fonction::Stop);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(Communicator_TourneAuDegresX)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            bool retour = communicator.tourneAuDegresX(150);

            Assert::IsTrue(communicator.itEcrire == 2);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::Tourne);
            Assert::IsTrue(communicator.ecriture[1] == 150);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(Communicator_TourneGauche)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
                 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire()  {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            bool retour = communicator.tourneGauche(150);

            Assert::IsTrue(communicator.itEcrire == 2);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::Gauche);
            Assert::IsTrue(communicator.ecriture[1] == 150);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(Communicator_TourneDroite)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            bool retour = communicator.tourneDroite(150);

            Assert::IsTrue(communicator.itEcrire == 2);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::Droite);
            Assert::IsTrue(communicator.ecriture[1] == 150);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(Communicator_ObtenirOrientation)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 234;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            int retour = communicator.obtenirOrientation();

            Assert::IsTrue(communicator.itEcrire == 1);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::Orientation);
            Assert::IsTrue(retour == 234);
        }

        TEST_METHOD(Communicator_SetDebug)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            communicator.setDebug();

            Assert::IsTrue(communicator.itEcrire == 1);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::SetDebug);
        }

        TEST_METHOD(Communicator_StopDebug)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            communicator.stopDebug();

            Assert::IsTrue(communicator.itEcrire == 1);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::StopDebug);
        }

        TEST_METHOD(Communicator_ResetErreur)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            communicator.resetErreur();

            Assert::IsTrue(communicator.itEcrire == 1);
            Assert::IsTrue(communicator.ecriture[0] == ArduinoCommunicator::Fonction::ResetErreur);
        }

        static void verifierLectureErreur(int param[4])
        {
            Assert::IsTrue(param[0] == ArduinoCommunicator::Fonction::Erreur);
            Assert::IsTrue(param[1] == 25);
        }

        TEST_METHOD(Communicator_SetEtStopFonctionLecture)
        {
            class ArduinoCommunicator_tst : public ArduinoCommunicator
            {
				 virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
				 virtual int lire() {
                    return itLecture++ % 2 ? Fonction::Erreur : 25;
                }
            public:
                int itEcrire = 0;
                int itLecture = 0;
                int ecriture[2];
            } communicator;

            communicator.setFonctionLecture(verifierLectureErreur);
            communicator.stopFonctionLecture();

            Assert::IsTrue(communicator.itEcrire == 0);
        }
    };
}