#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Software/SonarDriver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SoftwareTest
{		
	TEST_CLASS(SonarDriverTest)
	{
	public:
		
		TEST_METHOD(UpdateAndGetDist)
		{
			static unsigned int expected1 = 0,
								expected2 = 100;
			class ret100 : public ISonar
			{
				virtual unsigned int ping_cm() { return expected2; }
			} sonar;

			SonarDriver driver(&sonar);

			Assert::AreEqual(expected1, driver.getDist());
			driver.updateDist();
			Assert::AreEqual(expected2, driver.getDist());
		}

		TEST_METHOD(Obstacle)
		{
			bool expected = true;
			static unsigned int distance = 4;
			class ret100 : public ISonar
			{
				virtual unsigned int ping_cm() { return distance; }
			} sonar;

			SonarDriver driver(&sonar);
			driver.updateDist();

			Assert::AreEqual(expected, driver.isObstacle());
		}

		TEST_METHOD(PasObstacle)
		{
			bool expected = false;
			static unsigned int distance = 5;
			class ret100 : public ISonar
			{
				virtual unsigned int ping_cm() { return distance; }
			} sonar;

			SonarDriver driver(&sonar);
			driver.updateDist();

			Assert::AreEqual(expected, driver.isObstacle());
		}

		TEST_METHOD(Escalier)
		{
			bool expected = true;
			static unsigned int distance = 100;
			class ret100 : public ISonar
			{
				virtual unsigned int ping_cm() { return distance; }
			} sonar;

			SonarDriver driver(&sonar);
			driver.updateDist();

			Assert::AreEqual(expected, driver.isObstacle());
		}

	};
}