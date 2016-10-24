#include <iostream>
#include "../CommunicatorArduino/ArduinoCommunicator.h"

using namespace std;

int main(int argc, char **argv)
{
	ArduinoCommunicator communicator;
    if (!communicator.init())
    {
        cout << "Erreur lors de l'ouverture du port..." << endl;
        cin.get();

        return -1;
    }

    cout << "Orientation: " << communicator.obtenirOrientation() << endl;

	cin.get();
    return 0;
}