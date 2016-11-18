#include "Carte.h"

Carte::Carte()
{
    std::fstream Fichier;
    std::string line;
    Fichier.open(Config::Instance().GetString("FICHIER_DE_LA_CARTE"));

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
    RayonDeFusion = std::stof(Config::Instance().GetString("DISTANCE_ENTRE_POINT_DE_SEGMENTS"));
    AngleMaximum = std::stof(Config::Instance().GetString("ANGLE_ENTRE_LES_SEGMENTS"));
}

Carte::~Carte()
{
    //SaveCarte();
}

void Carte::FusionCarte(std::vector<segment>& nouveauSegments)
{
    bool *ignorer = new bool[nouveauSegments.size()]();

    for(int i = 0; i < nouveauSegments.size(); i++)
    {

        for(int j = 0; j < segments.size(); ++j)
        {
            if(segments[j].DisancePoint(nouveauSegments[i].debut) < RayonDeFusion ||
               segments[j].DisancePoint(nouveauSegments[i].fin) < RayonDeFusion )
            {
                if((nouveauSegments[i].fin - nouveauSegments[i].debut).normalize().dot((segments[j].fin - segments[j].debut).normalize()) < AngleMaximum)
                {
                    ignorer[i] = true;
                }
            }
        }
    }
    for(int i = 0; i < nouveauSegments.size(); i++)
    {
        if(!ignorer[i])
            segments.push_back(nouveauSegments[i]);
    }
    //segments.insert(segments.end(), nouveauSegments.begin(), nouveauSegments.end());

    float RayonDeFusion2 = RayonDeFusion * RayonDeFusion;
    for(int i = 0; i < segments.size(); ++i)
    {
        for(int j = 0; j < segments.size(); ++j)
        {
            if(j == i) continue;

            if(segments[i].fin.distanceFast(segments[j].debut) < RayonDeFusion2)
            {
                Vector2 PointCommun = (segments[i].fin - segments[j].debut) / 2 + segments[j].debut;
                segments[j].debut = segments[i].fin = PointCommun;
            }
            if(segments[i].fin.distanceFast(segments[j].fin) < RayonDeFusion2)
            {
                Vector2 PointCommun = (segments[i].fin - segments[j].fin) / 2 + segments[j].fin;
                segments[j].fin = segments[i].fin = PointCommun;
            }
        }
    }

    delete ignorer;
}

void Carte::SaveCarte()
{
    std::fstream Fichier;
    std::string line;
    Fichier.open(Config::Instance().GetString("FICHIER_DE_LA_CARTE"));

    if(Fichier.is_open())
    {
        for(int i = 0; i < segments.size(); ++i)
        {
            Fichier << segments[i].ToString();
        }
        Fichier.close();
    }
}
