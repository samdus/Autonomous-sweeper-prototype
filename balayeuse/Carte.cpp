#include "Carte.h"

Carte::Carte()
{
    std::fstream Fichier;
    std::string line;
    Fichier.open(FichierCarte);

    if(Fichier.is_open())
    {
        while ( getline (Fichier, line) )
        {
            segment seg;
            seg.fromString(line);
            segments.push_back(seg);
        }

        Fichier.close();
    }
}

Carte::~Carte()
{

}

void Carte::SaveCarte()
{
    std::fstream Fichier;
    std::string line;
    Fichier.open(FichierCarte);

    if(Fichier.is_open())
    {
        for(int i = 0; i < segments.size(); ++i)
        {
            Fichier << segments[i].ToString();
        }
        Fichier.close();
    }
}
