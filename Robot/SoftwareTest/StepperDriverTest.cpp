#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Software/StepperDriver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{
	TEST_CLASS(StepperDriverTest)
	{
	public:

		TEST_METHOD(SetVitesse)
		{
			const unsigned short expected = 2;

			StepperDriver driver(NULL);
			driver.setVitesse(expected);
			Assert::IsTrue(expected == driver.getVitesse());
		}

		TEST_METHOD(SetAvant)
		{
			const char expected = 'A';

			StepperDriver driver(NULL);
			driver.avant();
			Assert::AreEqual(expected, driver.getDirection());
		}

		TEST_METHOD(SetArriere)
		{
			const char expected = 'R';

			StepperDriver driver(NULL);
			driver.derriere();
			Assert::AreEqual(expected, driver.getDirection());
		}

		TEST_METHOD(SetGauche)
		{
			const char expected = 'G';

			StepperDriver driver(NULL);
			driver.gauche();
			Assert::AreEqual(expected, driver.getDirection());
		}

		TEST_METHOD(SetDroite)
		{
			const char expected = 'D';

			StepperDriver driver(NULL);
			driver.droite();
			Assert::AreEqual(expected, driver.getDirection());
		}

		TEST_METHOD(SetAvance)
		{
			const bool expected = false;

			StepperDriver driver(NULL);
			driver.avance();
			Assert::AreEqual(expected, driver.getImmobile());
		}

		TEST_METHOD(SetStop)
		{
			const bool expected = true;

			StepperDriver driver(NULL);
			driver.stop();
			Assert::AreEqual(expected, driver.getImmobile());
		}

		TEST_METHOD(StepImmobile)
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
			StepperDriver driver(&stepper);

			driver.step();

			Assert::AreEqual(expected, stepper.compte);
		}

		TEST_METHOD(StepVitesseMax)
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

			StepperDriver driver(&stepper);
			driver.avance();

			for (int i = 0; i < nbTour; ++i)
				driver.step();

			Assert::AreEqual(expected, stepper.compte);
		}

		TEST_METHOD(StepVitesseMin)
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

			StepperDriver driver(&stepper);

			driver.setVitesse(0);
			driver.avance();

			for (int i = 0; i < nbTour; ++i)
				driver.step();

			Assert::AreEqual(expected, stepper.compte);
		}
	};
}