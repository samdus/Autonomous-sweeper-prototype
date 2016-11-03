#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Software/CompassDriver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{
	TEST_CLASS(CompassDriverTest)
	{
	public:
		TEST_METHOD(Compass_getOrientationMemeLectureChaqueFois)
		{
			static float expected = 0.75;
			class retPoint75 : public ICompass
			{
				virtual bool init() { return true; }
				 virtual float read() { return expected; }
			} compas;

			CompassDriver driver(&compas);
			float diff = expected - driver.getOrientation();
			Assert::IsTrue(diff > -0.0001 && diff < 0.0001);
		}

		TEST_METHOD(Compass_getOrientationAvecMedianne)
		{
			static float expected = 175.f;
			static float lectures[COMPAS_NB_READ] = {200, 180, 175, 160, 175, 170, 174, 175, 176, 177};
			
			class retPoint75 : public ICompass
			{
				int it = 0;
				virtual bool init() { return true; }
				virtual float read() { return lectures[it++]; }
			} compas;

			CompassDriver driver(&compas);
			float retour = driver.getOrientation();
			float diff = expected - retour;
			Assert::IsTrue(diff > -0.0001 && diff < 0.0001);
		}
	};
}