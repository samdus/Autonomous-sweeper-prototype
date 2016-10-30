#include "Convertisseur.h"

struct thread_arg
{
    CloudPointContainer* cloudBuffer;
    Convertisseur* parent;
};

Convertisseur::Convertisseur()
{
    ContinuerConvertion = true;
    InitilasationConfig();
}

Convertisseur::~Convertisseur()
{

}

std::vector<Vector3> EspionnageVoisinage(std::vector<Vector3>& ensembleDePoints, int& pointSource, float distance)
{
    distance = distance * distance;
    std::vector<Vector3> voisin = std::vector<Vector3>();
    for(int i = 0;i < ensembleDePoints.size(); ++i)
    {
        if(distance > ensembleDePoints[i].distanceXZfast(ensembleDePoints[pointSource]))
        {
            voisin.push_back(ensembleDePoints[i]);
        }
    }
    return voisin;
}


void EtendreCluster(std::vector<Vector3>& ensembleDePoints,std::vector<Flag>& flags, int point, std::vector<Vector3>& pointVoisins, std::vector<Vector3>& cluster, float distance, int minimumPoints)
{
    cluster.push_back(ensembleDePoints[point]);
    for(int i = 0; i < pointVoisins.size(); ++i)
    {
        if(flags[i] != Flag::VISITE)
        {
            flags[i] = Flag::VISITE;
            std::vector<Vector3> pointVoisin = EspionnageVoisinage(ensembleDePoints, i, distance);
            if(pointVoisin.size() >= minimumPoints)
            {
                //std::merge (pointVoisins.begin(),pointVoisins.end(),pointVoisin.begin(),pointVoisins.end(),pointVoisins.begin());//p-t pas necessaire
            }
        }
        if(std::find(cluster.begin(), cluster.end(), ensembleDePoints[i]) == cluster.end())
        {
            cluster.push_back(ensembleDePoints[i]);
        }
    }
}

std::vector<std::vector<Vector3>> dbscan(std::vector<Vector3>& ensembleDePoints, float distance, int minimumPoints)
{
    std::vector<std::vector<Vector3>> Clusters = std::vector<std::vector<Vector3>>();
    std::vector<Vector3>Cluster = std::vector<Vector3>();
    std::vector<Flag> flags = std::vector<Flag>(ensembleDePoints.size(), Flag::NONE);

    for(int i = 0; i < ensembleDePoints.size(); ++i)
    {
        if(flags[i] == Flag::VISITE)
        {
            continue;
        }
        flags[i] = Flag::VISITE;
        std::vector<Vector3> pointVoisin = EspionnageVoisinage(ensembleDePoints, i, distance);
        if(pointVoisin.size() < minimumPoints)
        {
            flags[i] = Flag::BRUIT;
        }
        else
        {
            EtendreCluster(ensembleDePoints,flags, i, pointVoisin, Cluster, distance, minimumPoints);
            Clusters.push_back(Cluster);
        }
    }

    return Clusters;
}
void Convertisseur::DemarreThread(CloudPointContainer* donnees)
{
    static struct thread_arg args;
    args.cloudBuffer = donnees;
    args.parent = this;
    pthread_create(&convertisseur_thread, NULL, Convertir, (void*)&args);
}

void Convertisseur::InitilasationConfig()
{
    dist_max = std::stof(Config::Instance().GetString("TRESHOLD_BETWEEN_POINT"));
    MinNumberOfPoint = std::stoi(Config::Instance().GetString("MIN_NUMBER_OF_POINT"));
}

void* Convertisseur::Convertir(void* parent)
{
    struct thread_arg* convertisseur = (struct thread_arg*)parent;
    std::vector<Vector3> points = std::vector<Vector3>();
    int indiceTraite = -1;

    while(convertisseur->parent->ContinuerConvertion)
    {
        points = std::vector<Vector3>();
        indiceTraite = convertisseur->cloudBuffer->GetCopyCloudPointToConvert(points);

        if(indiceTraite != -1)
        {
            std::cout << "Nombre de point a traite : " << points.size() << std::endl;
            std::vector<std::vector<Vector3>> Clusters = dbscan(points, convertisseur->parent->dist_max, convertisseur->parent->MinNumberOfPoint);
            std::cout << "Nombre de Cluster trouve : " << Clusters.size() << std::endl;
            convertisseur->cloudBuffer->Converted[indiceTraite] = true;
            for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
            {
                std::cout << convertisseur->cloudBuffer->Converted[indiceTraite] << " ";
            }
            std::cout << std::endl << "Fin" << std::endl;
        }
    }
}

Vector3 Convertisseur::FromString(std::string text)
{
    Vector3 vec = Vector3();
    int deb = 0;
    int pos = text.find(" ");

    vec.x = std::stof(text.substr(0,pos));
    deb = pos + 1;
    pos = text.find(" ",deb);
    vec.y = std::stof(text.substr(deb,pos));
    deb = pos + 1;
    pos = text.find(" ",deb);
    vec.z = std::stof(text.substr(deb,pos));

    return vec;
}

void Convertisseur::ConvertiFichier()
{
    std::vector<Vector3> points = std::vector<Vector3>();
    std::fstream Fichier;
    std::string line;
    Fichier.open(Config::Instance().GetString("FICHIER_DE_POINT"));

    if(Fichier.is_open())
    {
        while ( getline (Fichier, line) )
        {
            points.push_back(FromString(line));
        }

        Fichier.close();
    }

    std::cout << "Nombre de point a traite : " << points.size() << std::endl;
    std::vector<std::vector<Vector3>> Clusters = dbscan(points, dist_max, MinNumberOfPoint);
    std::cout << "Nombre de Cluster trouve : " << Clusters.size() << std::endl;
}
