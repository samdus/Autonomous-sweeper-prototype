#include <iostream>
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
		if(debug[1] == ArduinoCommunicator::TypeErreur::Obstacle)
			cout << "Obstacle!!" << endl;
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

	communicator.setFonctionLecture(afficherDebug);

	communicator.avancePendantXDixiemeSec(30);
	
	cin.get();
    return 0;
}