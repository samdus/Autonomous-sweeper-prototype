#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include<Windows.h>

#include "../Software/ControlleurPrincipal.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{
	TEST_CLASS(ControlleurPrincipalRotationTest)
	{
	public:
		static void attendre(unsigned long) {}
		static void transmettreDonnee(int, bool) {}

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
				TestCase( 70.f, 185, droite),
				TestCase( 70.f, 260, gauche),
				TestCase(271.f,  89, droite),
				TestCase( 25.f, 280, gauche),
				TestCase( 24.f,  26, rien),
				TestCase( 26.f,  24, rien),
				TestCase(359.f,   1, rien),
				TestCase(  1.f, 359, rien),
				TestCase(270.f, 180, gauche),
				TestCase(180.f, 270, droite)
			};

			static TestCase casCourant = cases[0];

			for (TestCase cas : cases)
			{
				casCourant = cas;
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
					tstCtrl() : ControlleurPrincipal(NULL, NULL, NULL, &compasDriverCustom) { _transmettreDonnee = transmettreDonnee; }
				protected:
					virtual bool tourneADroiteVersXDegres(int16_t) { compteurDroite++; return true; }
					virtual bool tourneAGaucheVersXDegres(int16_t) { compteurGauche++; return true; }
				} ctrlTest;

				Assert::IsTrue(ctrlTest.tourneAuDegresX(casCourant.destination), casCourant.message);

				if(casCourant.sens == gauche)
					Assert::AreEqual(1, ctrlTest.compteurGauche, casCourant.message);
				else if(casCourant.sens == droite)
					Assert::AreEqual(1, ctrlTest.compteurDroite, casCourant.message);
				else
				{
					Assert::AreEqual(0, ctrlTest.compteurGauche, casCourant.message);
					Assert::AreEqual(0, ctrlTest.compteurDroite, casCourant.message);
				}

				delete[] casCourant.message;
			}
		}

		TEST_METHOD(TournerGauche_BonneDecisionDepart)
		{
			struct TestCase
			{
				float lectureCompas[5];
				int16_t destination;
				const wchar_t* message;

				TestCase(float depart, int16_t dest, float steps[3]) {
					lectureCompas[0] = depart;
					for (int i = 1; i <= 3; ++i)
						lectureCompas[i] = steps[i];
					lectureCompas[4] = dest;

					destination = dest;

					static int nb = 0;
					std::string str = "Test #" + std::to_string(++nb) + " de " + std::to_string((int)depart) + " a " + std::to_string(dest);
					int wchars_num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
					wchar_t* wstr = new wchar_t[wchars_num];
					MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, wchars_num);

					message = wstr;
				}
			};

			TestCase cases[] = {
				TestCase( 70.f, 260, new float[3] {  85, 150, 230 }),
				TestCase( 25.f, 280, new float[3] {  30, 150, 250 }),
				TestCase(270.f, 180, new float[3] { 225, 200, 270 })
			};

			static TestCase casCourant = cases[0];

			for (TestCase cas : cases)
			{
				casCourant = cas;

				class CompasDriverCustom : public CompassDriver
				{
				public:
					int compteur = 0;
					CompasDriverCustom() : CompassDriver(NULL) { }
					virtual bool init() { return true; }
					virtual float getOrientation() {
						return casCourant.lectureCompas[compteur++];
					}
				} compasDriverCustom;

				class dumbStepper : public StepperDriver
				{
				public:
					int compteur = 0;
					dumbStepper() : StepperDriver(NULL, 1) {}
					virtual void gauche() { compteur++; }
					virtual void avance() { compteur++; }
				} dummyStepper;

				static dumbStepper* ds;
				static CompasDriverCustom* cd;

				ds = &dummyStepper;
				cd = &compasDriverCustom;

				class tstCtrl : public ControlleurPrincipal
				{
				public:
					tstCtrl() : ControlleurPrincipal(ds, ds, NULL, cd) { _attendre = attendre; }
					bool test(int16_t a) { return tourneAGaucheVersXDegres(a); }
				} ctrlTest;

				Assert::IsTrue(ctrlTest.test(casCourant.destination), casCourant.message);
				Assert::AreEqual(5, compasDriverCustom.compteur, casCourant.message);
				Assert::AreEqual(4, dummyStepper.compteur, casCourant.message);

				delete[] casCourant.message;
			}
		}

		TEST_METHOD(TournerDroite_BonneDecisionDepart)
		{
			struct TestCase
			{
				float lectureCompas[5];
				int16_t destination;
				const wchar_t* message;

				TestCase(float depart, int16_t dest, float steps[3]) {
					lectureCompas[0] = depart;
					for (int i = 1; i <= 3; ++i)
						lectureCompas[i] = steps[i];
					lectureCompas[4] = dest;

					destination = dest;

					static int nb = 0;
					std::string str = "Test #" + std::to_string(++nb) + " de " + std::to_string((int)depart) + " a " + std::to_string(dest);
					int wchars_num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
					wchar_t* wstr = new wchar_t[wchars_num];
					MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, wchars_num);

					message = wstr;
				}
			};

			TestCase cases[] = {
				TestCase( 70.f, 185,  new float[3]{  85, 150, 230 }),
				TestCase(271.f,  89,  new float[3]{ 200, 125, 100 }),
				TestCase(180.f, 270,  new float[3]{ 190, 200, 225 })
			};

			static TestCase casCourant = cases[0];

			for (TestCase cas : cases)
			{
				casCourant = cas;

				class CompasDriverCustom : public CompassDriver
				{
				public:
					int compteur = 0;
					CompasDriverCustom() : CompassDriver(NULL) { }
					virtual bool init() { return true; }
					virtual float getOrientation() {
						return casCourant.lectureCompas[compteur++];
					}
				} compasDriverCustom;

				class dumbStepper : public StepperDriver
				{
				public:
					int compteur = 0;
					dumbStepper() : StepperDriver(NULL, 1) {}
					virtual void droite() { compteur++; }
					virtual void avance() { compteur++; }
				} dummyStepper;

				static dumbStepper* ds;
				static CompasDriverCustom* cd;

				ds = &dummyStepper;
				cd = &compasDriverCustom;

				class tstCtrl : public ControlleurPrincipal
				{
				public:
					tstCtrl() : ControlleurPrincipal(ds, ds, NULL, cd) { _attendre = attendre; }
					bool test(int16_t a) { return tourneADroiteVersXDegres(a); }
				} ctrlTest;

				Assert::IsTrue(ctrlTest.test(casCourant.destination), casCourant.message);
				Assert::AreEqual(5, compasDriverCustom.compteur, casCourant.message);
				Assert::AreEqual(4, dummyStepper.compteur, casCourant.message);

				delete[] casCourant.message;
			}
		}

		TEST_METHOD(TournerGauche_MauvaiseDecisionDepart)
		{
			struct TestCase
			{
				float lectureCompas[5];
				int16_t destination;
				const wchar_t* message;

				TestCase(float depart, int16_t dest, float steps[3]) {
					lectureCompas[0] = depart;
					for (int i = 1; i <= 3; ++i)
						lectureCompas[i] = steps[i];
					lectureCompas[4] = dest;

					destination = dest;

					static int nb = 0;
					std::string str = "Test #" + std::to_string(++nb) + " de " + std::to_string((int)depart) + " a " + std::to_string(dest);
					int wchars_num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
					wchar_t* wstr = new wchar_t[wchars_num];
					MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, wchars_num);

					message = wstr;
				}
			};

			TestCase cases[] = {
				TestCase(70.f, 260, new float[3]{ 60, 55, 50 }),
				TestCase(25.f, 280, new float[3]{ 30, 150, 250 }),
				TestCase(270.f, 180, new float[3]{ 225, 200, 270 })
			};

			static TestCase casCourant = cases[0];

			for (TestCase cas : cases)
			{
				casCourant = cas;

				class CompasDriverCustom : public CompassDriver
				{
				public:
					int compteur = 0;
					CompasDriverCustom() : CompassDriver(NULL) { }
					virtual bool init() { return true; }
					virtual float getOrientation() {
						return casCourant.lectureCompas[compteur++];
					}
				} compasDriverCustom;

				class dumbStepper : public StepperDriver
				{
				public:
					int compteur = 0;
					dumbStepper() : StepperDriver(NULL, 1) {}
					virtual void gauche() { compteur++; }
					virtual void avance() { compteur++; }
				} dummyStepper;

				static dumbStepper* ds;
				static CompasDriverCustom* cd;

				ds = &dummyStepper;
				cd = &compasDriverCustom;

				class tstCtrl : public ControlleurPrincipal
				{
				public:
					tstCtrl() : ControlleurPrincipal(ds, ds, NULL, cd) { _attendre = attendre; }
					bool test(int16_t a) { return tourneAGaucheVersXDegres(a); }
				} ctrlTest;

				Assert::IsTrue(ctrlTest.test(casCourant.destination), casCourant.message);
				Assert::AreEqual(5, compasDriverCustom.compteur, casCourant.message);
				Assert::AreEqual(4, dummyStepper.compteur, casCourant.message);

				delete[] casCourant.message;
			}
		}
	};
}