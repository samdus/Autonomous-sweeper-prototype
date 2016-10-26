#include <iostream>
#include "RotationTestsFonctionnels.h"
#include "../CommunicatorArduino/ArduinoCommunicator.h"

using namespace std;

void afficherDebug(int debug[4])
{
	switch (debug[0])
	{
	case ArduinoCommunicator::Fonction::InfoDistanceObjet:
		cout << "Distance: " << debug[1] << endl;
		break;
	case ArduinoCommunicator::Fonction::InfoOrientation:
		cout << "Orientation: " << debug[1] << endl;
		break;
	case ArduinoCommunicator::Fonction::InfoVitesseMoteur:
		cout << "Vitesse du moteur: " << debug[1] << endl;
		break;
	case ArduinoCommunicator::Fonction::Erreur:
		switch (debug[1])
		{
		case ArduinoCommunicator::TypeErreur::Obstacle:
			cout << "Obstacle!!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::FonctionInconnue:
			cout << "Fonction inconnue!!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::IO:
			cout << "Erreur de IO" << endl;
			break;
		}
		break;
	}
}

int main(int argc, char **argv)
{
	ArduinoCommunicator communicator;
    if (!communicator.init())
    {
        cout << "Erreur lors de l'ouverture du port..." << endl;
        cin.get();

        return -1;
    }
/*
	communicator.setFonctionLecture(afficherDebug);

	//	Pourra éventuellement testé si la rotation fonctionne
	//	----------------------------------------------------------
	//	RotationTestsFonctionels rotation;
	//	rotation.test(communicator);

	communicator.tourneGauchePendant(20);
	communicator.tourneDroitePendant(20);
*/
	cin.get();
    return 0;
}