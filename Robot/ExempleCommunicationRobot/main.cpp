#include <iostream>
#include <bitset>
#include "../CommunicatorArduino/ArduinoCommunicator.h"

using namespace std;

//Fonction qui servira d'affichage
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
	//Déclaration du communicator pour communiquer avec le Arduino
	ArduinoCommunicator communicator;

	//Initialisation (il faut indiquer la fonction qui servira d'affichage)
    if (!communicator.init(afficherDebug))
    {
		cout << endl << "Erreur lors de l'ouverture du port..." << endl;
        cin.get();

        return -1;
    }
	
	/*
	Différentes commandes sont possibles:
	------------------------------------------------------

	// - Afficher l'angle à l'infini
	while (1)
		cout << "Angle: " << communicator.obtenirOrientation() << endl;
	
	// - Afficher la distance de l'objet devant à l'infini
	while (1)
		cout << "Dist: " << communicator.obtenirDistanceDevant() << endl;

	// - Afficher s'il y a un obstacle ou non (à l'infini)
	//while (1)
	//	cout << "Obstacle: " << (communicator.obtenirObstacle() ? "Oui" : "Non") << endl;

	// - Tourner pendant un certain temps
	communicator.tourneDroitePendant(55);
	communicator.tourneGauchePendant(55);

	// - Activer le debug
	communicator.setDebug();

	// - Avancer pendant un certain temps
	communicator.avancePendantXDixiemeSec(19000);

	// - Tourner à un certain angle (ne fonctionne pas dans l'état actuel)
	communicator.tourneDroite(120);
	*/

	return 0;
}