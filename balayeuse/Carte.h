#pragma once

#include <iostream>
#include <fstream>
#include "../lib/Segment.h"
#include <string>
#include <vector>
#include "Config.h"

class Carte
{
    private:
    std::vector<segment> segments;
    float RayonDeFusion;
    float AngleMaximum;

    public:
    Carte();
    ~Carte();

    void FusionCarte(std::vector<segment>& nouveauSegments);
    std::vector<segment>& GetSegments() { return segments; }

    void SaveCarte();
};
