#ifndef SEGMENT
#define SEGMENT
#include "Vectors.h"
#include <string>

class segment
{
 public:
    Vector2 debut;
    Vector2 fin;
    int nbPoint;

    segment() {debut = Vector2(); fin = Vector2(); nbPoint = 0;}
    ~segment() {}

    float LongueurFast()
    {
        return fin.distanceFast(debut);
    }

    float DisancePoint(Vector2 point)
    {
        float m, b = 0.0;
        if(fin.x == debut.x)
        {
            m = 0;
        }
        else
        {
            m = (fin.y - debut.y) / (fin.x - debut.x);
        }
        b = fin.y - m * fin.x;

        return std::abs(m * point.x - point.y + b)/ sqrtf(m*m+1);
    }

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
#endif
