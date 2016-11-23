#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "CloudPointContainer.h"
#include "Config.h"
#include "Carte.h"
#include <algorithm>

enum Flag
{
    NONE,
    VISITE,
    BRUIT
};

class Convertisseur
{
    private:
    pthread_t convertisseur_thread;
    float dist_max;
    int MinNumberOfPoint;
    float distMinPourCassure;
    float distMaxPourFusion;
    int nbPointParVertice;
    bool enleverbruit;

    static void* ConvertirThread(void* parent);
    Vector3 FromString(std::string text);

public :
    Carte Environnement;
    bool ContinuerConvertion;
    std::vector<std::vector<Vector3>> Clusters;

    Convertisseur();
    ~Convertisseur();

    void InitialisationConfig(Config& ConfigHelper);
    void Convertir(CloudPointContainer& cloudBuffer);
    void ConvertiFichier();

    void DemarreThread(CloudPointContainer* donnees);

};
