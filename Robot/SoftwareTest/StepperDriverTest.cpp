#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Software/StepperDriver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{
	TEST_CLASS(StepperDriverTest)
	{
	public:

		TEST_METHOD(Stepper_SetVitesse)
		{
			const unsigned short expected = 2;

			StepperDriver driver(NULL, 0);
			driver.setVitesse(expected);
			Assert::IsTrue(expected == driver.getVitesse());
		}

		TEST_METHOD(Stepper_SetAvant)
		{
			const char expected[2] = { -1 , -1 };

			StepperDriver driver1(NULL, 0);
			StepperDriver driver2(NULL, 1);

			driver1.avant();
			driver2.avant();

			Assert::AreEqual(expected[0], driver1.getDirection());
			Assert::AreEqual(expected[1], driver2.getDirection());
		}

		TEST_METHOD(Stepper_SetArriere)
		{
			const char expected[2] = { 1 , 1 };

			StepperDriver driver1(NULL, 0);
			StepperDriver driver2(NULL, 1);

			driver1.derriere();
			driver2.derriere();

			Assert::AreEqual(expected[0], driver1.getDirection());
			Assert::AreEqual(expected[1], driver2.getDirection());
		}

		TEST_METHOD(Stepper_SetGauche)
		{
			const char expected[2] = { 1 , -1 };

			StepperDriver driver1(NULL, 0);
			StepperDriver driver2(NULL, 1);

			driver1.gauche();
			driver2.gauche();

			Assert::AreEqual(expected[0], driver1.getDirection());
			Assert::AreEqual(expected[1], driver2.getDirection());
		}

		TEST_METHOD(Stepper_SetDroite)
		{
			const char expected[2] = { -1 , 1 };

			StepperDriver driver1(NULL, 0);
			StepperDriver driver2(NULL, 1);

			driver1.droite();
			driver2.droite();

			Assert::AreEqual(expected[0], driver1.getDirection());
			Assert::AreEqual(expected[1], driver2.getDirection());
		}

		TEST_METHOD(Stepper_SetAvance)
		{
			const bool expected = true;

			StepperDriver driver(NULL, 0);
			driver.avance();
			Assert::AreEqual(expected, driver.isEnMouvement());
		}

		TEST_METHOD(Stepper_SetStop)
		{
			const bool expected = false;

			StepperDriver driver(NULL, 0);
			driver.stop();
			Assert::AreEqual(expected, driver.isEnMouvement());
		}

		TEST_METHOD(Stepper_StepImmobile)
		{
			class Stepp : public IStepper
			{
			public:
				int compte = 0;
				 virtual void init(byte, byte, byte, byte) {}
				 virtual void nextStep(char direction) { compte++; }
				 virtual	void motorPinsOut(byte pins) {}
			} stepper;

			int expected = 0;
			StepperDriver driver(&stepper, 0);

			driver.step();

			Assert::AreEqual(expected, stepper.compte);
		}

		TEST_METHOD(Stepper_StepVitesseMax)
		{
			class Stepp : public IStepper
			{
			public:
				int compte = 0;
				 virtual void init(byte, byte, byte, byte) {}
				 virtual void nextStep(char direction) { compte++; }
				 virtual	void motorPinsOut(byte pins) {}
			} stepper;

			int nbTour = 32;
			int expected = nbTour;

			StepperDriver driver(&stepper, 0);
			driver.avance();

			for (int i = 0; i < nbTour; ++i)
				driver.step();

			Assert::AreEqual(expected, stepper.compte);
		}

		TEST_METHOD(Stepper_StepVitesseMin)
		{
			class Stepp : public IStepper
			{
			public:
				int compte = 0;
				 virtual void init(byte, byte, byte, byte) {}
				 virtual void nextStep(char direction) { compte++; }
				 virtual	void motorPinsOut(byte pins) {}
			} stepper;

			int nbTour = 32;
			int expected = nbTour / 2;

			StepperDriver driver(&stepper, 0);

			driver.setVitesse(0);
			driver.avance();

			for (int i = 0; i < nbTour; ++i)
				driver.step();

			Assert::AreEqual(expected, stepper.compte);
		}

        TEST_METHOD(Stepper_StepVitesseLimites)
        {
            class Stepp : public IStepper
            {
            public:
				 virtual void init(byte, byte, byte, byte) {}
				 virtual void nextStep(char direction) {}
				 virtual	void motorPinsOut(byte pins) {}
            } stepper;

            StepperDriver driver(&stepper, 0);

            driver.setVitesse(10);
            Assert::IsTrue(driver.getVitesse() == 8);
        }
	};
}