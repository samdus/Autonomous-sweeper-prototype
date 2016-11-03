#include <iostream>
#include <bitset>
#include <random>
#include <string>
#include "../CommunicatorArduino/ArduinoCommunicator.h"
#include "../../mongoWrapper/MongoWrapper.cpp"

using namespace std;

string EnvoiWeb = "";

void afficherDebug(int16_t debug[4])
{
	switch (debug[0])
	{
	/*case ArduinoCommunicator::Fonction::InfoDistanceObjet:
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
		break;*/
	case ArduinoCommunicator::Fonction::DirectionChoisie:
		cout << debug[1];
		EnvoiWeb += std::to_string(debug[1]);
		break;
	/*case ArduinoCommunicator::Fonction::Erreur:
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
		break;*/
	}
}

int getSensSuppose(int depart, int arrive)
{
	int diff;
	diff = arrive - depart;

	if (fabs(diff) <= CTRL_PRINC_DIFF_ANGLE_ACCEPTE || fabs(diff) >= 360 - CTRL_PRINC_DIFF_ANGLE_ACCEPTE)
	{
		return 0;
	}
	else if ((diff < 0 && diff > -180) || diff > 180)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

int main(int argc, char **argv)
{
	const int NB_TEST = 1000000;

	ArduinoCommunicator communicator;
	MongoWrapper mongoWrapper;

    if (!communicator.init(afficherDebug))
    {
		cout << endl << "Erreur lors de l'ouverture du port..." << endl;
        cin.get();

        return -1;
    }

	static std::random_device seeder;
	static std::mt19937 rng(seeder());
	static std::uniform_int_distribution<int> gen(0, 359);

	cout << "================================" << endl
		 << "            DEBUT               " << endl
		 << "================================" << endl;

	cout << "Traitement initial, sens: ";
	communicator.tourneAuDegresX(0);
	cout << endl;

	int depart = 0;
	int destination;
	int sensSuppose;

	for (int i = 0; i < NB_TEST; ++i)
	{
		destination = gen(rng);
		sensSuppose = getSensSuppose(depart, destination);
		cout << sensSuppose;
		EnvoiWeb += std::to_string(sensSuppose);

		communicator.tourneAuDegresX(destination);

		cout << " " << depart << " " << destination << endl;
		EnvoiWeb += " " + std::to_string(depart) + " " + std::to_string(destination);

		mongoWrapper.writeConsole("Test " + std::to_string(i) + ") " + EnvoiWeb, "info");

		depart = destination;
	}

	cout << "===============================" << endl
		 << "            FIN                " << endl 
		 << "===============================" << endl;
	//cin.get();

	//Attendre que les donnees sont 
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	Sleep(1000);
#else
	usleep(1000 * 1000); // takes microseconds
#endif
	return 0;
}