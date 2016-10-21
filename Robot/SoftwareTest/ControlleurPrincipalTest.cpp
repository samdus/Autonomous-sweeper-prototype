
#include <functional>
#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Software/ControlleurPrincipal.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{

    TEST_CLASS(ControlleurPrincipalTest)
    {
    public:

        static class DummyStepper : public IStepper
        {
        public:
            virtual void init(byte, byte, byte, byte) {}
            virtual void nextStep(char direction) {}
            virtual	void motorPinsOut(byte pins) {}
        } dummyStepper;

        static class DummySonar : public ISonar
        {
        public:
            int sonarPing = 8;
            virtual unsigned int ping_cm() { return sonarPing; }
        } dummySonnar;

        static class DummyCompas : public ICompass
        {
            int compasOrientation = 0;
            virtual void init() {}
            virtual float read() { return compasOrientation; }
        } dummyCompas;

        class DummyStepperDriver : public StepperDriver
        {
        public:
            DummyStepperDriver(int m) : StepperDriver(&dummyStepper, m) { };
        };

        class DummySonarDriver : public SonarDriver
        {
        public:
            DummySonarDriver() : SonarDriver(&dummySonnar) { };
        };

        class DummyCompasDriver : public CompassDriver
        {
        public:
            DummyCompasDriver() : CompassDriver(&dummyCompas) { };
        };

        byte pinsMoteurGauche[4] = { 0 };
        byte pinsMoteurDroit[4] = { 0 };

        DummyStepperDriver *stepperDriverGauche = new DummyStepperDriver(STEPPER_GAUCHE), 
                           *stepperDriverDroit  = new DummyStepperDriver(STEPPER_DROIT);
        DummySonarDriver *sonarDriver = new DummySonarDriver();
        DummyCompasDriver *compasDriver = new DummyCompasDriver();

        static void transmettreDonnee(int) {}
        static void attendre(unsigned long) {}

        TEST_METHOD(ControlleurPrincipal_AvancePendantXDixiemeSec)
        {
            ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver);

            auto att  = [controlleur](unsigned long temp) {
                
            };

            controlleur.init(transmettreDonnee, reinterpret_cast<std::function<void()>*>(att), pinsMoteurGauche, pinsMoteurDroit);
            controlleur.avancePendantXDixiemeSec(100);
/*
            bool retour = communicator.avancePendantXDixiemeSec(100);

            Assert::IsTrue(communicator.itEcrire == 2);
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::Avance);
            Assert::IsTrue(communicator.ecriture[1] == 100);
            Assert::IsTrue(retour);*/
        }

        TEST_METHOD(ControlleurPrincipal_ReculePendantXDixiemeSec)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::Recule);
            Assert::IsTrue(communicator.ecriture[1] == 100);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(ControlleurPrincipal_Stop)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture == ControlleurPrincipal::Fonction::Stop);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(ControlleurPrincipal_TourneAuDegresX)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::Tourne);
            Assert::IsTrue(communicator.ecriture[1] == 150);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(ControlleurPrincipal_TourneGauche)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
            {
                virtual void ecrire(int message) { ecriture[itEcrire++] = message; }
                virtual int lire() {
                    return 1;
                }
            public:
                int itEcrire = 0;
                int ecriture[2];
            } communicator;

            bool retour = communicator.tourneGauche(150);

            Assert::IsTrue(communicator.itEcrire == 2);
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::Gauche);
            Assert::IsTrue(communicator.ecriture[1] == 150);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(ControlleurPrincipal_TourneDroite)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::Droite);
            Assert::IsTrue(communicator.ecriture[1] == 150);
            Assert::IsTrue(retour);
        }

        TEST_METHOD(ControlleurPrincipal_ObtenirOrientation)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::Orientation);
            Assert::IsTrue(retour == 234);
        }

        TEST_METHOD(ControlleurPrincipal_SetDebug)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::SetDebug);
        }

        TEST_METHOD(ControlleurPrincipal_StopDebug)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::StopDebug);
        }

        TEST_METHOD(ControlleurPrincipal_ResetErreur)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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
            Assert::IsTrue(communicator.ecriture[0] == ControlleurPrincipal::Fonction::ResetErreur);
        }

        static void verifierLectureErreur(int param[4])
        {
            Assert::IsTrue(param[0] == ControlleurPrincipal::Fonction::Erreur);
            Assert::IsTrue(param[1] == 25);
        }

        TEST_METHOD(ControlleurPrincipal_SetEtStopFonctionLecture)
        {
            class ArduinoControlleurPrincipal_tst : public ControlleurPrincipal
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