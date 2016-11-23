#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "Decodeur.h"
#include "../mongoWrapper/MongoWrapper.cpp"
MongoWrapper test;

Decodeur DecodeurScene = Decodeur();

void printInfo()
{
    std::cout << "\nAvailable Controls:"              << std::endl;
    std::cout << "==================="                << std::endl;
    std::cout << "Quit         :   Ctrl + C\n"        << std::endl;
}

int main(int argc, char **argv)
{
    DecodeurScene.Init();
	test.writeConsole("Test from raspberry", "warning");
    printInfo();

    while(1)
    {
        DecodeurScene.RunLoop();
    }

    return 0;
}
