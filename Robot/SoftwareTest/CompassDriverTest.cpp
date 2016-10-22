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
				volatile virtual void init()volatile {}
				volatile virtual float read()volatile { return expected; }
			} compas;

			CompassDriver driver(&compas);
			float diff = expected - driver.getOrientation();
			Assert::IsTrue(diff > -0.0001 && diff < 0.0001);
		}

		TEST_METHOD(Compass_getOrientationAvecMoyenne)
		{
			static float expected = 0.745f;
			
			class retPoint75 : public ICompass
			{
				float retour = expected - 0.005f - (COMPAS_NB_READ / 200.f);
				volatile virtual void init()volatile {}
				volatile virtual float read()volatile { retour += 0.01f; return retour; }
			} compas;

			CompassDriver driver(&compas);
			float diff = expected - driver.getOrientation();
			Assert::IsTrue(diff > -0.0001 && diff < 0.0001);
		}
	};
}