#include "stdafx.h"
#include "CppUnitTest.h"
#include<Windows.h>

#include "../Software/ControlleurPrincipal.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{
    TEST_CLASS(ControlleurPrincipalTest)
    {
    public:

        class DummyStepper : public IStepper
        {
        public:
			 virtual void init(byte, byte, byte, byte) {}
			 virtual void nextStep(char direction)  {}
			 virtual void motorPinsOut(byte pins) {}
        };

        class DummySonar : public ISonar
        {
        public:
            int sonarPing = 8;
			virtual unsigned int ping_cm() { return sonarPing; }
        };

        class DummyCompas : public ICompass
        {
            float compasOrientation = 0;
			virtual bool init() { return true; }
			virtual float read() { return compasOrientation; }
        };

		class DummyStepperDriver : public StepperDriver
		{
		public:
			unsigned short vitesse = 8;

			DummyStepperDriver(int m) : StepperDriver(new DummyStepper(), m) { };

			virtual void setVitesse(unsigned short v) {
				vitesse = v; 
			}
			virtual const unsigned short getVitesse() const { return vitesse; }
        };

        class DummySonarDriver : public SonarDriver
        {
        public:
            DummySonarDriver() : SonarDriver(new DummySonar()) { };
        };

        class DummyCompasDriver : public CompassDriver
        {
        public:
            DummyCompasDriver() : CompassDriver(new DummyCompas()) { };
        };

        byte pinsMoteurGauche[4] = { 0 };
        byte pinsMoteurDroit[4] = { 0 };

        DummyStepperDriver *stepperDriverGauche = new DummyStepperDriver(STEPPER_GAUCHE), 
                           *stepperDriverDroit  = new DummyStepperDriver(STEPPER_DROIT);
        DummySonarDriver *sonarDriver = new DummySonarDriver();
        DummyCompasDriver *compasDriver = new DummyCompasDriver();

        static void transmettreDonnee(int,bool) {}
		static void resetTemps() {};
        static void attendre(unsigned long) {}
		static unsigned long obtenirTemps() { return 0L; }
		
		int* retourFonction = NULL;
		void(*fonctionAsync)(ControlleurPrincipal&);		

        TEST_METHOD(ControlleurPrincipal_AvancePendantXDixiemeSec)
        {
            ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver, &retourFonction, &fonctionAsync);

            controlleur.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurGauche, pinsMoteurDroit);
			controlleur.avancePendantXDixiemeSec(100);

			Assert::IsTrue(stepperDriverGauche->isEnMouvement());
			Assert::IsTrue(stepperDriverDroit->isEnMouvement());

			Assert::AreEqual(-1, (int)stepperDriverGauche->getDirection());
			Assert::AreEqual(-1, (int)stepperDriverDroit->getDirection());
        }
		
        TEST_METHOD(ControlleurPrincipal_ReculePendantXDixiemeSec)
        {
			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver, &retourFonction, &fonctionAsync);

			controlleur.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurGauche, pinsMoteurDroit);
			controlleur.reculePendantXDixiemeSec(100);

			Assert::IsTrue(stepperDriverGauche->isEnMouvement());
			Assert::IsTrue(stepperDriverDroit->isEnMouvement());

			Assert::AreEqual(1, (int)stepperDriverGauche->getDirection());
			Assert::AreEqual(1, (int)stepperDriverDroit->getDirection());
        }
		
        TEST_METHOD(ControlleurPrincipal_ObtenirOrientation)
        {
			static float expected = 90;

			class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				 virtual float getOrientation() {
					return expected;
				}
			} compasDriverCustom;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom, &retourFonction, &fonctionAsync);
			controlleur.obtenirOrientation();

			Assert::AreEqual((int)expected, *retourFonction);
			delete retourFonction;
        }

        TEST_METHOD(ControlleurPrincipal_SetDebug)
        {
			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver, &retourFonction, &fonctionAsync);
			controlleur.setDebug();

			Assert::IsTrue(controlleur.isDebug());
        }

        TEST_METHOD(ControlleurPrincipal_StopDebug)
        {
			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver, &retourFonction, &fonctionAsync);
			controlleur.setDebug();

			Assert::IsTrue(controlleur.isDebug());
        }

		TEST_METHOD(ControlleurPrincipal_StepMoteur)
		{
			class TestStepperDriver : public StepperDriver
			{
			public:
				int nbCall = 0;
				TestStepperDriver(int m) : StepperDriver(new DummyStepper(), m) { };
				 void step() { nbCall++; }
			};
			TestStepperDriver testDriverGauche(STEPPER_GAUCHE),
							  testDriverDroit(STEPPER_DROIT);

			ControlleurPrincipal controlleur(&testDriverGauche, &testDriverDroit, sonarDriver, compasDriver, &retourFonction, &fonctionAsync);
			controlleur.stepMoteur();

			Assert::AreEqual(1, testDriverGauche.nbCall);
			Assert::AreEqual(1, testDriverDroit.nbCall);
		}

		TEST_METHOD(ControlleurPrincipal_CalibrerMoteur)
		{
			struct test_case
			{
				float orientationDepart,
					orientationEnsuite;
				int forceGaucheDepart,
					forceDroitDepart,
					forceGaucheEnsuite,
					forceDroitEnsuite;
				bool avant;
				test_case() {};
				test_case(float od, float oe, int fgd, int fge, int fdd, int fde, bool a)
				{
					orientationDepart = od;
					orientationEnsuite = oe;
					forceGaucheDepart = fgd;
					forceGaucheEnsuite = fge;
					forceDroitDepart = fdd;
					forceDroitEnsuite = fde;
					avant = a;
				}
			};

			test_case cases[8] = {
				test_case(60,61.1,8,8,8,8, true),  //AvantErreurMineure
				test_case(60,61.1,8,8,8,8, false), //DerriereErreurMineure
				test_case(90,95,8,8,8,7, true),    //AvantErreurVersGauche
				test_case(95,90,8,7,8,8, true),    //AvantErreurVersDroite
				test_case(90,95,8,7,8,8, false),   //DerriereErreurVersGauche
				test_case(95,90,8,8,8,7, false),   //DerriereErreurVerDroite
				test_case(90,95,7,8,8,8, true),	   //AvantErreurVersGauche_MonterMoteur
				test_case(95,90,8,8,7,8, true)	   //AvantErreurVersDroite_MonterMoteur
			};

			static int nbCallGetOrientation = 0;
			static test_case* test = nullptr;
			static ControlleurPrincipal* ctrl = nullptr;
			static StepperDriver* stepperGauche = nullptr;
			static StepperDriver* stepperDroit = nullptr;

			for (test_case t : cases)
			{
				test = &t;
				nbCallGetOrientation = 1;

				class TestCompassDriver : public CompassDriver
				{
				public:
					int compteur = 1;
					TestCompassDriver() : CompassDriver(new DummyCompas()) { };

					 virtual float getOrientation() {
						if (compteur++ == 1)
						{
							return test->orientationDepart;
						}
						else
						{
							return test->orientationEnsuite;
						}
					}
				} testCompasDriver;

				DummyStepperDriver testStepperDriverGauche(STEPPER_GAUCHE),
								   testStepperDriverDroit(STEPPER_DROIT);

				ControlleurPrincipal controlleur(&testStepperDriverGauche, &testStepperDriverDroit, sonarDriver, &testCompasDriver, &retourFonction, &fonctionAsync);

				testStepperDriverGauche.setVitesse(test->forceGaucheDepart);
				testStepperDriverDroit.setVitesse(test->forceDroitDepart);

				ctrl = &controlleur;
				stepperGauche = &testStepperDriverGauche;
				stepperDroit = &testStepperDriverDroit;

				controlleur.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurGauche, pinsMoteurDroit);

				if(test->avant)
					controlleur.avancePendantXDixiemeSec(100);
				else
					controlleur.reculePendantXDixiemeSec(100);

				Assert::IsTrue(stepperGauche->getVitesse() == (unsigned short)test->forceGaucheDepart);
				Assert::IsTrue(stepperDroit->getVitesse() == (unsigned short)test->forceDroitDepart);

				controlleur.calibrerMoteur();

				Assert::AreEqual(test->forceGaucheEnsuite, (int)testStepperDriverGauche.getVitesse());
				Assert::AreEqual(test->forceDroitEnsuite, (int)testStepperDriverDroit.getVitesse());
			}
		}

		TEST_METHOD(Rotation_Choix)
		{
			struct TestCase
			{
				float lectureCompas;
				int16_t destination;
				int sens;
				const wchar_t* message;

				TestCase(float lecture, int16_t dest, int sense) {
					lectureCompas = lecture;
					destination = dest;
					sens = sense;

					static int nb = 0;
					std::string str = "Test #" + std::to_string(++nb) + " de " + std::to_string((int)lecture) + " a " + std::to_string(dest);
					int wchars_num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
					wchar_t* wstr = new wchar_t[wchars_num];
					MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, wchars_num);

					message = wstr;
				}
			};

			const int gauche = 1;
			const int droite = 2;
			const int rien = 3;
			TestCase cases[] = {
				TestCase(70.f, 185, droite),
				TestCase(70.f, 260, gauche),
				TestCase(271.f,  89, droite),
				TestCase(25.f, 280, gauche),
				TestCase(24.f,  26, rien),
				TestCase(26.f,  24, rien),
				TestCase(359.f,   1, rien),
				TestCase(1.f, 359, rien),
				TestCase(270.f, 180, gauche),
				TestCase(180.f, 270, droite)
			};

			static TestCase casCourant = cases[0];
			static StepperDriver* stepGauche;
			static StepperDriver* stepDroit;
			static SonarDriver* sonDriv = sonarDriver;

			for (TestCase cas : cases)
			{
				casCourant = cas;
				stepGauche = new DummyStepperDriver(STEPPER_GAUCHE);
				stepDroit = new DummyStepperDriver(STEPPER_DROIT);

				static class CompasDriverCustom : public CompassDriver
				{
					int compteur = 0;
				public:
					CompasDriverCustom() : CompassDriver(NULL) { }
					virtual bool init() { return true; }
					virtual float getOrientation() {
						return casCourant.lectureCompas;
					}
				} compasDriverCustom;

				class tstCtrl : public ControlleurPrincipal
				{
				public:
					int compteurDroite = 0, compteurGauche = 0;
					tstCtrl() : ControlleurPrincipal(stepGauche, stepDroit, sonDriv, &compasDriverCustom, &retourFonction, &fonctionAsync) { _transmettreDonnee = transmettreDonnee; }
				protected:
					int* retourFonction = NULL;
					void(*fonctionAsync)(ControlleurPrincipal&);

					virtual bool tourneADroiteVersXDegres(int16_t) { compteurDroite++; return true; }
					virtual bool tourneAGaucheVersXDegres(int16_t) { compteurGauche++; return true; }
				} ctrlTest;

				ctrlTest.tourneAuDegresX(casCourant.destination);

				if (casCourant.sens == gauche)
				{
					Assert::IsTrue(stepGauche->isEnMouvement(), casCourant.message);
					Assert::IsTrue(stepDroit->isEnMouvement(), casCourant.message);

					Assert::AreEqual(1, (int)stepGauche->getDirection(), casCourant.message);
					Assert::AreEqual(-1, (int)stepDroit->getDirection(), casCourant.message);
				}
				else if (casCourant.sens == droite)
				{
					Assert::IsTrue(stepGauche->isEnMouvement(), casCourant.message);
					Assert::IsTrue(stepDroit->isEnMouvement(), casCourant.message);

					Assert::AreEqual(-1, (int)stepGauche->getDirection(), casCourant.message);
					Assert::AreEqual(1, (int)stepDroit->getDirection(), casCourant.message);
				}
				else
				{
					Assert::IsFalse(stepGauche->isEnMouvement(), casCourant.message);
					Assert::IsFalse(stepDroit->isEnMouvement(), casCourant.message);
				}
				
				delete stepGauche;
				delete stepDroit;
				delete[] casCourant.message;
			}
		}

		TEST_METHOD(VerifierDestinationRotationContinue)
		{
			static float anglePresent = 90,
						 angleDestination = 200;

			static StepperDriver* stepGauche = stepperDriverGauche;
			static StepperDriver* stepDroit = stepperDriverDroit;
			static SonarDriver* sonDriv = sonarDriver;
			static CompassDriver* comDriv = compasDriver;

			static class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				virtual float getOrientation() {
					return anglePresent;
				}
			} compasDriverCustom;
			
			class TstCtrl : public ControlleurPrincipal
			{
			public:
				int compteurDroite = 0, compteurGauche = 0;
				TstCtrl() : ControlleurPrincipal(stepGauche, stepDroit, sonDriv, &compasDriverCustom, &retourFonction, &fonctionAsync) { _destinationRotation = angleDestination; }
				int* retourFonction = NULL;
				void(*fonctionAsync)(ControlleurPrincipal&);

				static void verifierDestinationRotation(ControlleurPrincipal &ctrl) {
					ControlleurPrincipal::verifierDestinationRotation(ctrl);
				}
			} ctrlTest;
			
			ctrlTest.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurGauche, pinsMoteurDroit);
			stepGauche->avance();
			stepDroit->avance();
			
			TstCtrl::verifierDestinationRotation(ctrlTest);

			Assert::IsTrue(stepGauche->isEnMouvement());
			Assert::IsTrue(stepDroit->isEnMouvement());
		}

		TEST_METHOD(VerifierDestinationRotationArrete)
		{
			static float anglePresent = 198,
						 angleDestination = 200;

			static StepperDriver* stepGauche = stepperDriverGauche;
			static StepperDriver* stepDroit = stepperDriverDroit;
			static SonarDriver* sonDriv = sonarDriver;
			static CompassDriver* comDriv = compasDriver;

			static class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				virtual float getOrientation() {
					return anglePresent;
				}
			} compasDriverCustom;

			class TstCtrl : public ControlleurPrincipal
			{
			public:
				int compteurDroite = 0, compteurGauche = 0;
				TstCtrl() : ControlleurPrincipal(stepGauche, stepDroit, sonDriv, &compasDriverCustom, &retourFonction, &fonctionAsync) { _destinationRotation = angleDestination; }
				int* retourFonction = NULL;
				void(*fonctionAsync)(ControlleurPrincipal&);

				static void verifierDestinationRotation(ControlleurPrincipal &ctrl) {
					ControlleurPrincipal::verifierDestinationRotation(ctrl);
				}
			} ctrlTest;

			ctrlTest.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurGauche, pinsMoteurDroit);
			stepGauche->avance();
			stepDroit->avance();

			TstCtrl::verifierDestinationRotation(ctrlTest);

			Assert::IsFalse(stepGauche->isEnMouvement());
			Assert::IsFalse(stepDroit->isEnMouvement());
		}

		TEST_METHOD(VerifierTempsMouvementLineaireContinue)
		{
			static unsigned long tempsVoulu  = 250,
								 tempsEcoule = 200;

			static StepperDriver* stepGauche = stepperDriverGauche;
			static StepperDriver* stepDroit = stepperDriverDroit;
			static SonarDriver* sonDriv = sonarDriver;
			static CompassDriver* comDriv = compasDriver;

			static auto obtenirTemps = []()
			{
				return tempsEcoule;
			};

			class TstCtrl : public ControlleurPrincipal
			{
			public:
				int compteurDroite = 0, compteurGauche = 0;
				TstCtrl() : ControlleurPrincipal(stepGauche, stepDroit, sonDriv, comDriv, &retourFonction, &fonctionAsync) { _tempMouvementLineaire = tempsVoulu; _obtenirTemps = obtenirTemps; }
				int* retourFonction = NULL;
				void(*fonctionAsync)(ControlleurPrincipal&);

				static void verifierTempsMouvementLineaire(ControlleurPrincipal &ctrl) {
					ControlleurPrincipal::verifierTempsMouvementLineaire(ctrl);
				}
			} ctrlTest;

			ctrlTest.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurGauche, pinsMoteurDroit);
			stepGauche->avance();
			stepDroit->avance();

			TstCtrl::verifierTempsMouvementLineaire(ctrlTest);

			Assert::IsTrue(stepGauche->isEnMouvement());
			Assert::IsTrue(stepDroit->isEnMouvement());
		}

		TEST_METHOD(VerifierTempsMouvementLineaireArrete)
		{
			static unsigned long tempsVoulu = 250,
								 tempsEcoule = 250;

			static StepperDriver* stepGauche = stepperDriverGauche;
			static StepperDriver* stepDroit = stepperDriverDroit;
			static SonarDriver* sonDriv = sonarDriver;
			static CompassDriver* comDriv = compasDriver;

			static auto obtenirTemps = []()
			{
				return tempsEcoule;
			};

			class TstCtrl : public ControlleurPrincipal
			{
			public:
				int compteurDroite = 0, compteurGauche = 0;
				TstCtrl() : ControlleurPrincipal(stepGauche, stepDroit, sonDriv, comDriv, &retourFonction, &fonctionAsync) { _tempMouvementLineaire = tempsVoulu; _obtenirTemps = obtenirTemps; }
				int* retourFonction = NULL;
				void(*fonctionAsync)(ControlleurPrincipal&);

				static void verifierTempsMouvementLineaire(ControlleurPrincipal &ctrl) {
					ControlleurPrincipal::verifierTempsMouvementLineaire(ctrl);
				}
			} ctrlTest;

			ctrlTest.init(transmettreDonnee, resetTemps, obtenirTemps, pinsMoteurGauche, pinsMoteurDroit);
			stepGauche->avance();
			stepDroit->avance();

			TstCtrl::verifierTempsMouvementLineaire(ctrlTest);

			Assert::IsFalse(stepGauche->isEnMouvement());
			Assert::IsFalse(stepDroit->isEnMouvement());
		}
    };
}