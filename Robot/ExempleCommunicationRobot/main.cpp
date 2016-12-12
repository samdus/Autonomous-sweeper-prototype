#include <iostream>
#include <bitset>
#include "../CommunicatorArduino/ArduinoCommunicator.h"

using namespace std;

void afficherDebug(int16_t debug[4])
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
		if(debug[1] == 0)
			cout << "Vitesse du moteur gauche: " << debug[2] << endl;
		else
			cout << "Vitesse du moteur droit: " << debug[2] << endl;
		break;
	case ArduinoCommunicator::Fonction::DirectionChoisie:
		cout << debug[1];
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
			cout << "Erreur de IO!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::ErreurInitialisation:
			cout << "Erreur d'initialisation!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::EntreeInconnue:
			std::bitset<sizeof(int16_t) * 8> binaire(debug[2]);
			cout << "Entree inconnue: " << endl << binaire << endl;			
			break;
		}
		break;
	}
}

int main(int argc, char **argv)
{
	ArduinoCommunicator communicator;

    if (!communicator.init(afficherDebug))
    {
		cout << endl << "Erreur lors de l'ouverture du port..." << endl;
        cin.get();

        return -1;
    }
	
	/*while (1)
		cout << "Angle: " << communicator.obtenirOrientation() << endl;*/
	/*while (1)
		cout << "Dist: " << communicator.obtenirDistanceDevant() << endl;*/
	
	communicator.setDebug();

	cout << "Commence!" << endl;
	communicator.avancePendantXDixiemeSec(900);
	/*cout << "Commence a tourner a l'angle " << communicator.obtenirOrientation() << endl;
	communicator.tourneDroite(120);
	cout << "A tourne jusqu'a l'angle " << communicator.obtenirOrientation() << endl;
	communicator.avancePendantXDixiemeSec(500);*/
	cout << "Fin" << endl;

	return 0;
}