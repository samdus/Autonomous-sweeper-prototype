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
    std::string FichierCarte;

    public:
    Carte();
    ~Carte();
    void InitialisationConfig(Config& ConfigHelper);

    void FusionCarte(std::vector<segment>& nouveauSegments);
    std::vector<segment>& GetSegments() { return segments; }

    void SaveCarte();
};
