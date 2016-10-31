#include <iostream>
#include <bitset>
#include "RotationTestsFonctionnels.h"
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
        cout << "Erreur lors de l'ouverture du port..." << endl;
        cin.get();

        return -1;
    }
	
	cout << "Demande l'orientation..." << endl;
	cout << "Orientation: " << communicator.obtenirOrientation() << endl;

	//communicator.tourneDroitePendant(75);
	communicator.avancePendantXDixiemeSec(30);
	communicator.reculePendantXDixiemeSec(30);
	//communicator.tourneGauchePendant(75);

	cout << "Enter pour quitter." << endl;
	cin.get();
	return 0;
}












//	Pourra éventuellement tester si la rotation fonctionne
//	----------------------------------------------------------
//	RotationTestsFonctionels rotation;
//	rotation.test(communicator);


/*
Tests pour le port série
----------------------------------------------------------
serial::Serial *_serial;
string envoie = "test...";

try
{
_serial = new serial::Serial(ARDUINO_COMUNICATOR_PORT, ARDUINO_COMUNICATOR_BAUD, serial::Timeout::simpleTimeout(1000));
}
catch (serial::IOException e)
{
cout << "Erreur lors de l'ouverture de new serial::Serial..." << endl;
cin.get();

return -1;
}

try
{
_serial->write(envoie);
}
catch (serial::IOException e)
{
cout << "Erreur lors de l'ouverture de _serial->write..." << endl;
cin.get();

return -1;
}

try
{
string recu;

while (!_serial->available())
{

}

_serial->read(recu, envoie.length());

cout << "Recu: " << recu << endl;
}
catch (serial::IOException e)
{
cout << "Erreur lors de l'ouverture de _serial->write..." << endl;
cin.get();

return -1;
}


*/