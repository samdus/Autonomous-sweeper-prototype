#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "Decodeur.h"

Freenect::Freenect freenect;
Decodeur DecodeurScene = Decodeur();

void printInfo()
{
    std::cout << "\nAvailable Controls:"              << std::endl;
    std::cout << "==================="                << std::endl;
    std::cout << "Quit         :   Ctrl + C\n"        << std::endl;
}

int main(int argc, char **argv)
{
    DecodeurScene.Init(freenect.createDevice<MyFreenectDevice>(0));
    printInfo();

    while(1)
    {
        DecodeurScene.RunLoop();
    }

    return 0;
}
