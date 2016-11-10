#pragma once

#include <iostream>
#include <fstream>
#include "Vectors.h"
#include <string>
#include <vector>
#include "Config.h"

struct segment
{
    Vector2 debut;
    Vector2 fin;
    int nbPoint;

    segment() {debut = Vector2(); fin = Vector2(); nbPoint = 0;}
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
        fin.x = std::stof(text.substr(deb,pos));
        deb = pos + 1;
        pos = text.find(" ",deb);
        fin.y = std::stof(text.substr(deb,pos));
        deb = pos + 1;
        nbPoint = std::stoi(text.substr(deb));
    }

    std::string ToString()
    {
        return
        std::to_string(debut.x) + " " +
        std::to_string(debut.y) + " " +
        std::to_string(fin.x) + " " +
        std::to_string(fin.y) + " " +
        std::to_string(nbPoint) + "\n";
    }
};

class Carte
{
    private:
    std::vector<segment> segments;

    public:
    Carte();
    ~Carte();

    std::vector<segment>& GetSegments() { return segments; }

    void SaveCarte();
};
