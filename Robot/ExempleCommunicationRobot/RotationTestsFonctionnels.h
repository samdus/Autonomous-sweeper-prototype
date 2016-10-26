#ifndef ROTATION_TEST_FONCTIONNELS
#define ROTATION_TEST_FONCTIONNELS

#include <iostream>
#include <assert.h>

#include "../CommunicatorArduino/ArduinoCommunicator.h"

class RotationTestsFonctionels
{
private:
	struct testCaseTourne
	{
		std::string nomTest;
		int depart, angle, arrive, fonction;
		bool gauche;
		testCaseTourne(int d,int an,int arr,int f,bool g)
		{
			static int i = 1;
			nomTest = std::to_string(i++) + ") De " + std::to_string(d) + " a " + std::to_string(arr);
			depart = d; angle = an; arrive = arr; fonction = f;  gauche = g;
		}
	};

public:
	void test(ArduinoCommunicator &communicator) {
		testCaseTourne tests[] = {
			testCaseTourne( 90, 180, 270, 1, true),
			testCaseTourne(270, 180,  90, 1, true),
			testCaseTourne(270,  15, 285, 1, true),
			testCaseTourne( 90, 181, 271, 1, false),
			testCaseTourne(270, 181,  91, 1, false),
			testCaseTourne(270, 270, 180, 1, false),
			testCaseTourne( 90, 180, 270, 2, true),
			testCaseTourne(270, 180,  90, 2, true),
			testCaseTourne(270, 345, 285, 2, true),
			testCaseTourne( 90, 179, 271, 2, false),
			testCaseTourne(270, 179,  91, 2, false),
			testCaseTourne(270,  85, 185, 2, false),
			testCaseTourne( 90, 180, 270, 3, true),
			testCaseTourne(270, 180,  90, 3, true),
			testCaseTourne(270,  15, 285, 3, true),
			testCaseTourne( 90, 179, 271, 3, false),
			testCaseTourne(270, 179,  91, 3, false),
			testCaseTourne(270,  85, 185, 3, false)
		};
		static int angle = 0;
		static bool gauche = false;

		for (testCaseTourne test : tests)
		{
			switch (test.fonction)
			{
			case 1:
				assert(communicator.tourneGauche(test.angle) && test.nomTest.c_str());
				break;
			case 2:
				assert(communicator.tourneDroite(test.angle) && test.nomTest.c_str());
				break;
			case 3:
				assert(communicator.tourneAuDegresX(test.arrive) && test.nomTest.c_str());
				break;
			}

			int orientation = communicator.obtenirOrientation();
			assert(orientation > test.arrive - 1 && orientation < test.arrive + 1 && test.nomTest.c_str());
		}
	}
};

#endif // !ROTATION_TEST_FONCTIONNELS