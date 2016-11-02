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
/*
std::vector<int> EspionnageVoisinage(std::vector<Vector3>& ensembleDePoints, int& pointSource, float distanceMax)
{
    distanceMax = distanceMax * distanceMax;
    std::vector<int> voisin = std::vector<int>();
    for(int i = 0;i < ensembleDePoints.size(); ++i)
    {
        if(distanceMax > ensembleDePoints[i].distanceXZfast(ensembleDePoints[pointSource]))
        {
            voisin.push_back(i);
        }
        else //place pour optimisation ignorer a partir du moment que le vecteur ne touche plus au rayon
        {
            float ax = 0.0;
            float az = 0.0;
            float bx = ensembleDePoints[i].x * 1000; // gros chiffre pour assurer l'intersection
            float bz = ensembleDePoints[i].z * 1000; // gros chiffre pour assurer l'intersection
            float cx = ensembleDePoints[pointSource].x;
            float cz = ensembleDePoints[pointSource].z;

            float alpha = (bx-ax) * (bx-ax) + (bz-az) * (bz-az);
            float beta = 2 * ((bx-ax)*(ax-cx) + (bz-az)*(az-cz));
            float gama =  ax*ax + az*az + cx*cx + cz*cz -2*(ax*cx + az*cz) - distanceMax;

            //AVEC OPTIMISATION basee sur des assomption
            //float alpha = ensembleDePoints[i].x * ensembleDePoints[i].x + ensembleDePoints[i].z * ensembleDePoints[i].z;
            //float beta = 2 * (ensembleDePoints[i].x * (-ensembleDePoints[pointSource].x) + (-ensembleDePoints[pointSource].z));
            //float gama = ensembleDePoints[pointSource].x * ensembleDePoints[pointSource].x + ensembleDePoints[pointSource].z * ensembleDePoints[pointSource].z - distanceMax;

            if(beta*beta - 4*alpha*gama < 0) //Delta < 0
            {
                break;
            }
        }
    }
    return voisin;
}

void EtendreCluster(std::vector<Vector3>& ensembleDePoints,std::vector<Flag>& flags, int point, std::vector<int>& pointVoisins, std::vector<int>& cluster, float distance, int minimumPoints)
{
    cluster.push_back(point);
    for(int i = 0; i < pointVoisins.size(); ++i)
    {
        if(flags[i] != Flag::VISITE)
        {
            flags[i] = Flag::VISITE;
            std::vector<int> pointVoisin = EspionnageVoisinage(ensembleDePoints, i, distance);
            if(pointVoisin.size() >= minimumPoints)
            {
                pointVoisins.insert(pointVoisins.end(), pointVoisin.begin(), pointVoisin.end());
            }
        }
        if(std::find(cluster.begin(), cluster.end(), i) == cluster.end())
        {
            cluster.push_back(i);
        }
    }
}

std::vector<std::vector<int>> dbscan(std::vector<Vector3>& ensembleDePoints, float distance, int minimumPoints)
{
    std::vector<std::vector<int>> Clusters = std::vector<std::vector<int>>();
    std::vector<int>Cluster = std::vector<int>();
    std::vector<Flag> flags = std::vector<Flag>(ensembleDePoints.size(), Flag::NONE);

    for(int i = 0; i < ensembleDePoints.size(); ++i)
    {
        if(flags[i] == Flag::VISITE)
        {
            continue;
        }
        flags[i] = Flag::VISITE;
        std::vector<int> pointVoisin = EspionnageVoisinage(ensembleDePoints, i, distance);
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
*/
std::vector<std::vector<int>> DannyScan(std::vector<Vector3>& ensembleDePoints, float distance, int minimumPoints)
{
    float distanceMax = distance * distance;
    std::vector<std::vector<int>> Clusters = std::vector<std::vector<int>>();
    std::vector<int>Cluster = std::vector<int>();
    std::vector<Flag> flags = std::vector<Flag>(ensembleDePoints.size(), Flag::NONE);

    for(int i = 0; i < ensembleDePoints.size(); ++i)
    {
        if(flags[i] == Flag::VISITE)
        {
            continue;
        }

        flags[i] = Flag::VISITE;
        Cluster.push_back(i);
        for(int j = i; j < ensembleDePoints.size(); ++j)
        {
            if(flags[j] != Flag::VISITE)
            {
                //points potentiel
                if(distanceMax > ensembleDePoints[j].distanceXZfast(ensembleDePoints[Cluster.back()]))
                {
                    flags[j] = Flag::VISITE;
                    Cluster.push_back(j);
                }
                else
                {


                    float ax = 0.0;
                    float az = 0.0;
                    float bx = ensembleDePoints[j].x * 1000; // gros chiffre pour assurer l'intersection
                    float bz = ensembleDePoints[j].z * 1000; // gros chiffre pour assurer l'intersection
                    float cx = ensembleDePoints[Cluster.back()].x;
                    float cz = ensembleDePoints[Cluster.back()].z;

                    float alpha = (bx-ax) * (bx-ax) + (bz-az) * (bz-az);
                    float beta = 2 * ((bx-ax)*(ax-cx) + (bz-az)*(az-cz));
                    float gama =  ax*ax + az*az + cx*cx + cz*cz -2*(ax*cx + az*cz) - distanceMax;

                    //AVEC OPTIMISATION basee sur des assomption
                    //float alpha = ensembleDePoints[i].x * ensembleDePoints[i].x + ensembleDePoints[i].z * ensembleDePoints[i].z;
                    //float beta = 2 * (ensembleDePoints[i].x * (-ensembleDePoints[pointSource].x) + (-ensembleDePoints[pointSource].z));
                    //float gama = ensembleDePoints[pointSource].x * ensembleDePoints[pointSource].x + ensembleDePoints[pointSource].z * ensembleDePoints[pointSource].z - distanceMax;

                    if(beta*beta - 4*alpha*gama < 0) //Delta < 0
                    {
                        break;
                    }
                }
            }
        }

        if(Cluster.size() > minimumPoints)
        {
            Clusters.push_back(Cluster);
        }
        Cluster.clear();
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
            convertisseur->parent->Clusters = std::vector<std::vector<Vector3>>();
            std::cout << "Nombre de point a traite : " << points.size() << std::endl;
            std::vector<std::vector<int>> ClustersList = DannyScan(points, convertisseur->parent->dist_max, convertisseur->parent->MinNumberOfPoint);
            std::cout << "Nombre de Cluster trouve : " << ClustersList.size() << std::endl;
            std::cout << "Copy pour le debug..." << std::endl;
            for(int i = 0; i < ClustersList.size(); ++i)
            {
                convertisseur->parent->Clusters.push_back(std::vector<Vector3>());

                for(int j = 0; j < ClustersList[i].size(); ++j)
                {
                    //std::cout << "Copy de " << ClustersList[i][j] << " dans " << i << std::endl;
                    convertisseur->parent->Clusters[i].push_back(points[ClustersList[i][j]]);
                }
            }
            std::cout << "Fin de la copy" << std::endl;
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
    std::vector<std::vector<int>> Clusters = DannyScan(points, dist_max, MinNumberOfPoint);
    std::cout << "Nombre de Cluster trouve : " << Clusters.size() << std::endl;
}
