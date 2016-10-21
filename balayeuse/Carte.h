#pragma once

#include <iostream>
#include <fstream>
#include "Vectors.h"
#include <string>
#include <vector>

struct segment
{
    Vector3 debut;
    Vector3 fin;
    int nbPoint;

    void fromString(std::string text)
    {
        int deb = 0;
        int pos = text.find(" ");

        debut.x = std::stof(text.substr(0,pos));
        deb = pos + 1;
        pos = text.find(" ",deb);
        debut.y = std::stof(text.substr(deb,pos));
        deb = pos + 1;
        pos = text.find(" ",deb);
        debut.z = std::stof(text.substr(deb,pos));
        deb = pos + 1;
        pos = text.find(" ",deb);
        fin.x = std::stof(text.substr(deb,pos));
        deb = pos + 1;
        pos = text.find(" ",deb);
        fin.y = std::stof(text.substr(deb,pos));
        deb = pos + 1;
        pos = text.find(" ",deb);
        fin.z = std::stof(text.substr(deb,pos));
        deb = pos + 1;
        nbPoint = std::stoi(text.substr(deb));
    }

    std::string ToString()
    {
        return
        std::to_string(debut.x) + " " +
        std::to_string(debut.y) + " " +
        std::to_string(debut.z) + " " +
        std::to_string(fin.x) + " " +
        std::to_string(fin.y) + " " +
        std::to_string(fin.z) + " " +
        std::to_string(nbPoint) + "\n";
    }
};

class Carte
{
    private:
    std::vector<segment> segments;
    std::string FichierCarte = "Carte.txt";

    public:
    Carte();
    ~Carte();

    void SaveCarte();
};
