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
			 virtual void init() {}
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

        static void transmettreDonnee(int) {}
        static void attendre(unsigned long) {}

        TEST_METHOD(ControlleurPrincipal_AvancePendantXDixiemeSec)
        {
            ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

            auto att  = [](unsigned long temp) {
				Assert::AreEqual(-1, (int)stepperGauche->getDirection());
				Assert::AreEqual(-1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10000UL,temp);
            };

            controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
            Assert::IsTrue(controlleur.avancePendantXDixiemeSec(100));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());
        }
		
        TEST_METHOD(ControlleurPrincipal_ReculePendantXDixiemeSec)
        {
			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

			auto att = [](unsigned long temp) {
				Assert::AreEqual(1, (int)stepperGauche->getDirection());
				Assert::AreEqual(1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10000UL, temp);
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			Assert::IsTrue(controlleur.reculePendantXDixiemeSec(100));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());
        }

        TEST_METHOD(ControlleurPrincipal_TourneAuDegresXVersLaGauche)
        {
			static float angleDepart = 60, 
						 angleFin = 180;

			class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				 virtual float getOrientation() {
					return ++angleDepart;
				}
			} compasDriverCustom;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

			auto att = [](unsigned long temp) {
				Assert::AreEqual(1, (int)stepperGauche->getDirection());
				Assert::AreEqual(-1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10UL, temp);
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			Assert::IsTrue(controlleur.tourneAuDegresX(angleFin));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());

			Assert::AreEqual(angleFin, angleDepart);
        }
		
		TEST_METHOD(ControlleurPrincipal_TourneAuDegresXVersLaDroite)
		{
			static float angleDepart = 180,
						 angleFin = 60;

			class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				 virtual float getOrientation() {
					return --angleDepart;
				}
			} compasDriverCustom;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

			auto att = [](unsigned long temp) {
				Assert::AreEqual(-1, (int)stepperGauche->getDirection());
				Assert::AreEqual(1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10UL, temp);
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			Assert::IsTrue(controlleur.tourneAuDegresX(angleFin));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());

			Assert::AreEqual(angleFin, angleDepart);
		}

        TEST_METHOD(ControlleurPrincipal_TourneGauche_ReelGauche)
        {
			static float angleDepart = 60,
						 angleFin = 180;

			class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				 virtual float getOrientation() {
					return ++angleDepart;
				}
			} compasDriverCustom;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

			auto att = [](unsigned long temp) {
				Assert::AreEqual(1, (int)stepperGauche->getDirection());
				Assert::AreEqual(-1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10UL, temp);
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			Assert::IsTrue(controlleur.tourneGauche(angleFin - angleDepart));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());

			Assert::AreEqual(angleFin, angleDepart - 1);
        }

		TEST_METHOD(ControlleurPrincipal_TourneGauche_ReelDroite)
		{
			static float angleDepart = 180,
						 angleFin = 60;

			class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				 virtual float getOrientation() {
					return --angleDepart;
				}
			} compasDriverCustom;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

			auto att = [](unsigned long temp) {
				Assert::AreEqual(-1, (int)stepperGauche->getDirection());
				Assert::AreEqual(1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10UL, temp);
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			Assert::IsTrue(controlleur.tourneGauche(angleFin + angleDepart));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());

			Assert::AreEqual(angleFin, angleDepart + 3);
		}

		TEST_METHOD(ControlleurPrincipal_TourneDroite_ReelGauche)
		{
			static float angleDepart = 60,
						 angleFin = 180;

			class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				 virtual float getOrientation() {
					return ++angleDepart;
				}
			} compasDriverCustom;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

			auto att = [](unsigned long temp) {
				Assert::AreEqual(1, (int)stepperGauche->getDirection());
				Assert::AreEqual(-1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10UL, temp);
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			Assert::IsTrue(controlleur.tourneDroite(angleDepart + angleFin));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());

			Assert::AreEqual(angleFin, angleDepart - 1);
		}

		TEST_METHOD(ControlleurPrincipal_TourneDroite_ReelDroite)
		{
			static float angleDepart = 180,
						 angleFin = 60;

			class CompasDriverCustom : public CompassDriver
			{
			public:
				CompasDriverCustom() : CompassDriver(new DummyCompas()) { }
				 virtual float getOrientation() {
					return --angleDepart;
				}
			} compasDriverCustom;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom);
			static DummyStepperDriver* stepperGauche = stepperDriverGauche;
			static DummyStepperDriver* stepperDroit = stepperDriverDroit;

			auto att = [](unsigned long temp) {
				Assert::AreEqual(-1, (int)stepperGauche->getDirection());
				Assert::AreEqual(1, (int)stepperDroit->getDirection());

				Assert::IsTrue(stepperGauche->isEnMouvement());
				Assert::IsTrue(stepperDroit->isEnMouvement());

				Assert::AreEqual(10UL, temp);
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			Assert::IsTrue(controlleur.tourneDroite(angleDepart - angleFin));

			Assert::IsFalse(stepperGauche->isEnMouvement());
			Assert::IsFalse(stepperDroit->isEnMouvement());

			Assert::AreEqual(angleFin, angleDepart + 1);
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

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, &compasDriverCustom);
			Assert::AreEqual((int)expected, controlleur.obtenirOrientation());
        }

        TEST_METHOD(ControlleurPrincipal_SetDebug)
        {
			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver);
			controlleur.setDebug();

			Assert::IsTrue(controlleur.isDebug());
        }

        TEST_METHOD(ControlleurPrincipal_StopDebug)
        {
			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, sonarDriver, compasDriver);
			controlleur.setDebug();

			Assert::IsTrue(controlleur.isDebug());
        }

        TEST_METHOD(ControlleurPrincipal_VerifierObstacleEtResetErreur)
        {
			class TestSonarDriver : public SonarDriver
			{
			public:
				TestSonarDriver() : SonarDriver(new DummySonar()) { };

				 virtual bool isObstacle() const { return true; }
			} testSonarDriver;

			ControlleurPrincipal controlleur(stepperDriverGauche, stepperDriverDroit, &testSonarDriver, compasDriver);

			static ControlleurPrincipal* ctrl = &controlleur;
			auto att = [](unsigned long) 
			{
				ctrl->verifierObstacle();
			};

			controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);
			
			Assert::IsFalse(controlleur.avancePendantXDixiemeSec(5));
			Assert::IsTrue(controlleur.isErreur());

			controlleur.resetErreur();
			Assert::IsFalse(controlleur.isErreur());
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

			ControlleurPrincipal controlleur(&testDriverGauche, &testDriverDroit, sonarDriver, compasDriver);
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

				ControlleurPrincipal controlleur(&testStepperDriverGauche, &testStepperDriverDroit, sonarDriver, &testCompasDriver);

				testStepperDriverGauche.setVitesse(test->forceGaucheDepart);
				testStepperDriverDroit.setVitesse(test->forceDroitDepart);

				ctrl = &controlleur;
				stepperGauche = &testStepperDriverGauche;
				stepperDroit = &testStepperDriverDroit;

				auto att = [](unsigned long temp) {
					Assert::IsTrue(stepperGauche->getVitesse() == (unsigned short)test->forceGaucheDepart);
					Assert::IsTrue(stepperDroit->getVitesse() == (unsigned short)test->forceDroitDepart);

					ctrl->calibrerMoteur();
				};

				controlleur.init(transmettreDonnee, att, pinsMoteurGauche, pinsMoteurDroit);

				if(test->avant)
					Assert::IsTrue(controlleur.avancePendantXDixiemeSec(100));
				else
					Assert::IsTrue(controlleur.reculePendantXDixiemeSec(100));

				Assert::AreEqual(test->forceGaucheEnsuite, (int)testStepperDriverGauche.getVitesse());
				Assert::AreEqual(test->forceDroitEnsuite, (int)testStepperDriverDroit.getVitesse());
			}
		}
    };
}