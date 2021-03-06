#include "Convertisseur.h"

struct thread_arg
{
    CloudPointContainer* cloudBuffer;
    Convertisseur* parent;
};

Convertisseur::Convertisseur()
{
    ContinuerConvertion = true;
}

Convertisseur::~Convertisseur()
{

}

std::vector<std::vector<int>> DannyScan(std::vector<Vector3>& ensembleDePoints, float distance, size_t minimumPoints)
{
    float distanceMax = distance * distance;
    std::vector<std::vector<int>> Clusters = std::vector<std::vector<int>>();
    std::vector<int>Cluster = std::vector<int>();
    std::vector<Flag> flags = std::vector<Flag>(ensembleDePoints.size(), Flag::NONE);

    for(size_t i = 0; i < ensembleDePoints.size(); ++i)
    {
        if(flags[i] == Flag::VISITE)
        {
            continue;
        }

        flags[i] = Flag::VISITE;
        Cluster.push_back(i);
        for(size_t j = i; j < ensembleDePoints.size(); ++j)
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
    pthread_create(&convertisseur_thread, NULL, ConvertirThread, (void*)&args);
}

void Convertisseur::InitialisationConfig(Config& ConfigHelper)
{
    dist_max = std::stof(ConfigHelper.GetString("TRESHOLD_BETWEEN_POINT"));
    MinNumberOfPoint = std::stoi(ConfigHelper.GetString("MIN_NUMBER_OF_POINT"));
    distMinPourCassure = std::stof(ConfigHelper.GetString("DISTANCE_POUR_LE_SECTIONNEMENT"));
    nbPointParVertice = std::stoi(ConfigHelper.GetString("NOMBRE_DE_POINT_POUR_LES_VERTICES_DU_MESH"));
    enleverbruit = std::stoi(ConfigHelper.GetString("ENLEVE_BRUIT_DU_CLUSTER"));
    distMaxPourFusion = std::stof(ConfigHelper.GetString("DISTANCE_ENTRE_SEGMENT_POUR_FUSION"));
    Environnement.InitialisationConfig(ConfigHelper);
}

Vector2 MoyenneVector2(size_t index, std::vector<int>& ensIndex, std::vector<Vector3>& ensPoints, size_t nbPoint)
{
    Vector2 vect = Vector2();
    for(size_t i = 0; i < nbPoint; ++i)
    {
        vect.x += ensPoints[ensIndex[index + i]].x;
        vect.y += ensPoints[ensIndex[index + i]].z;
    }
    vect.x /= nbPoint;
    vect.y /= nbPoint;
    return vect;
}

std::vector<segment> CoupeSegment(segment& seg, std::vector<int>& ensIndex, std::vector<Vector3>& ensPoints, float dist, size_t nbPointParVertice)
{
    std::vector<segment> segments = std::vector<segment>();
    std::vector<int> debutSegment = std::vector<int>();
    std::vector<int> finSegment = std::vector<int>();
    segments.push_back(seg);
    debutSegment.push_back(0);
    finSegment.push_back(ensIndex.size()-1);



    size_t pointPlusLoin = 0;
    for(size_t i = 0; i < segments.size(); ++i)
    {
        float distPaP = 0.0;
        float distMax = dist;
        bool coupeSegment = false;
        float m, b; //y = mx + b

        if(seg.fin.x == seg.debut.x)
        {
            m = 0;
        }
        else
        {
            m = (segments[i].fin.y - segments[i].debut.y) / (segments[i].fin.x - segments[i].debut.x);
        }
        b = seg.fin.y - m * seg.fin.x;

        for(size_t j = debutSegment[i] + nbPointParVertice; j < size_t(finSegment[i]) && j < ensIndex.size() - nbPointParVertice; ++j)
        {
            distPaP = std::abs(m * ensPoints[ensIndex[j]].x - ensPoints[ensIndex[j]].z + b)/ sqrtf(m*m+1);
            if(distPaP > distMax)
            {
                distMax = distPaP;
                pointPlusLoin = j;
                coupeSegment = true;
            }
        }

        if(coupeSegment)
        {
            segment nouvSeg = segment();
            nouvSeg.debut = segments[i].debut;
            nouvSeg.fin = MoyenneVector2(pointPlusLoin - nbPointParVertice / 2, ensIndex, ensPoints, nbPointParVertice);//SOMETIME GENERATE SEG FAULT -> not the case from a long time
            nouvSeg.nbPoint = pointPlusLoin - debutSegment[i];

            segments[i].debut = nouvSeg.fin;
            segments[i].nbPoint -= nouvSeg.nbPoint;

            segments.insert(segments.begin() + i, nouvSeg);
            debutSegment.insert(debutSegment.begin() + i + 1, pointPlusLoin);
            finSegment.insert(finSegment.begin() + i, pointPlusLoin);

            //on doit recomencer du debut car il se pourrait qu'on doivent le resectionner
            --i;
        }
    }

    return segments;
}

void LisserSegment(std::vector<segment>& segments, float distMaxPourFusion)
{
    segment ligne = segments[0];
    bool Fusion = true;
    size_t i,j,k = 0;
    std::vector<segment> resultat = std::vector<segment>();
    for( i = 1; i < segments.size(); ++i)
    {
        ligne.fin = (segments[i].fin - segments[i].debut) / 2 + segments[i].debut;
        for(j = k; j < i; ++j)
        {
            if(ligne.DisancePoint(segments[j].fin) > distMaxPourFusion)
            {
                Fusion = false;
                break;
            }
        }
        if(!Fusion)
        {
            if(i-1 > k)
            {
                ligne.fin = (segments[i-1].fin - segments[i-1].debut) / 2 + segments[i-1].debut;
                for(j = k+1; j < i; ++j)
                {
                    ligne.nbPoint += segments[j].nbPoint;
                }
                resultat.push_back(ligne);
                ligne.debut = ligne.fin;
                k = i-1;
                --i;
                Fusion = true;
            }
            else
            {
                resultat.push_back(ligne);
                ligne.debut = ligne.fin;
            }
        }
    }
    ligne.fin = segments[segments.size() - 1].fin;
    resultat.push_back(ligne);

    segments.swap(resultat);
}

void EnleverBruit(std::vector<int>& index, std::vector<Vector3>& points)
{
    double moy = 0.0;
    double ecartType = 0.0;
    moy += points[index[0]].distanceXZfast(points[index[1]]) * 2;
    moy += points[index[index.size()-1]].distanceXZfast(points[index[index.size()-2]]) * 2;
    for(size_t i = 1; i < index.size() - 1; ++i)
    {
        moy += points[index[i]].distanceXZfast(points[index[i-1]]) + points[index[i]].distanceXZfast(points[index[i+1]]);
    }

    moy /= index.size();

    ecartType += ((points[index[0]].distanceXZfast(points[index[1]]) * 2) - moy) * ((points[index[0]].distanceXZfast(points[index[1]]) * 2) - moy);
    ecartType += ((points[index[index.size()-1]].distanceXZfast(points[index[index.size()-2]]) * 2) - moy) * ((points[index[index.size()-1]].distanceXZfast(points[index[index.size()-2]]) * 2) - moy);
    for(size_t i = 1; i < index.size() - 1; ++i)
    {
        ecartType += ((points[index[i]].distanceXZfast(points[index[i-1]]) + points[index[i]].distanceXZfast(points[index[i+1]])) - moy) * ((points[index[i]].distanceXZfast(points[index[i-1]]) + points[index[i]].distanceXZfast(points[index[i+1]])) - moy);
    }
    ecartType /= index.size();
    ecartType = sqrtf(ecartType);

    float minDist = moy - ecartType;
    float maxDist = moy + ecartType;
    for(size_t i = 1; i < index.size() - 1; ++i)
    {
        float distance = (points[index[i]].distanceXZfast(points[index[i-1]]) + points[index[i]].distanceXZfast(points[index[i+1]]));
        if(distance < minDist || distance > maxDist)
        {
            index.erase(index.begin()+i);
        }
    }
}

void* Convertisseur::ConvertirThread(void* parent)
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
            std::vector<std::vector<int>> ClustersList = DannyScan(points, convertisseur->parent->dist_max, convertisseur->parent->MinNumberOfPoint);
            for(size_t i = 0; i < ClustersList.size(); ++i)
            {
                if(convertisseur->parent->enleverbruit)
                {
                    EnleverBruit(ClustersList[i], points);
                }
            }

            {//DEBUG
                //std::cout << "Copy pour le debug..." << std::endl;
                /*for(size_t i = 0; i < ClustersList.size(); ++i)
                {
                    convertisseur->parent->Clusters.push_back(std::vector<Vector3>());

                    for(size_t j = 0; j < ClustersList[i].size(); ++j)
                    {
                        convertisseur->parent->Clusters[i].push_back(points[ClustersList[i][j]]);
                    }
                }*/
                //std::cout << "Fin de la copy" << std::endl;
            }

            //remet le flag a vrai pour indiquer que les points ont été traité
            convertisseur->cloudBuffer->Converted[indiceTraite] = true;

            //std::cout << "Debut de la generation du mesh..." << std::endl;
            for(size_t i = 0; i < ClustersList.size(); ++i)
            {
                segment seg = segment();

                seg.debut = MoyenneVector2(0, ClustersList[i], points, convertisseur->parent->nbPointParVertice);
                seg.fin = MoyenneVector2(ClustersList[i].size()-convertisseur->parent->nbPointParVertice, ClustersList[i], points, convertisseur->parent->nbPointParVertice);
                seg.nbPoint = ClustersList[i].size();

                std::vector<segment> segments = CoupeSegment(seg, ClustersList[i], points, convertisseur->parent->distMinPourCassure, convertisseur->parent->nbPointParVertice);
                LisserSegment(segments, convertisseur->parent->distMaxPourFusion);
                convertisseur->parent->Environnement.FusionCarte(segments);
            }
            //std::cout << "Fin de la generation du mesh..." << std::endl;
        }
    }
    return 0;
}

void Convertisseur::Convertir(CloudPointContainer& cloudBuffer)
{
    std::vector<Vector3> points = std::vector<Vector3>();
    int indiceTraite = -1;

    points = std::vector<Vector3>();
    indiceTraite = cloudBuffer.GetCopyCloudPointToConvert(points);

    if(indiceTraite != -1)
    {
        Clusters = std::vector<std::vector<Vector3>>();
        std::vector<std::vector<int>> ClustersList = DannyScan(points, dist_max, MinNumberOfPoint);
        for(size_t i = 0; i < ClustersList.size(); ++i)
        {
            if(enleverbruit)
            {
                EnleverBruit(ClustersList[i], points);
            }
        }

        {//DEBUG
            //std::cout << "Copy pour le debug..." << std::endl;
            /*for(size_t i = 0; i < ClustersList.size(); ++i)
            {
                Clusters.push_back(std::vector<Vector3>());

                for(size_t j = 0; j < ClustersList[i].size(); ++j)
                {
                    Clusters[i].push_back(points[ClustersList[i][j]]);
                }
            }*/
            //std::cout << "Fin de la copy" << std::endl;
        }

        //remet le flag a vrai pour indiquer que les points ont été traité
        cloudBuffer.Converted[indiceTraite] = true;

        //std::cout << "Debut de la generation du mesh..." << std::endl;
        for(size_t i = 0; i < ClustersList.size(); ++i)
        {
            segment seg = segment();

            seg.debut = MoyenneVector2(0, ClustersList[i], points, nbPointParVertice);
            seg.fin = MoyenneVector2(ClustersList[i].size()-nbPointParVertice, ClustersList[i], points, nbPointParVertice);
            seg.nbPoint = ClustersList[i].size();

            std::vector<segment> segments = CoupeSegment(seg, ClustersList[i], points, distMinPourCassure, nbPointParVertice);
            LisserSegment(segments, distMaxPourFusion);
            Environnement.FusionCarte(segments);
            //Environnement.GetSegments().insert(Environnement.GetSegments().end(), segments.begin(), segments.end());
        }
        //std::cout << "Fin de la generation du mesh..." << std::endl;
    }
}

Vector3 Convertisseur::FromString(std::string text)
{
    Vector3 vec = Vector3();
    size_t deb = 0;
    size_t pos = text.find(" ");

    vec.x = std::stof(text.substr(0,pos));
    deb = pos + 1;
    pos = text.find(" ",deb);
    vec.y = std::stof(text.substr(deb,pos));
    deb = pos + 1;
    pos = text.find(" ",deb);
    vec.z = std::stof(text.substr(deb,pos));

    return vec;
}

/*void Convertisseur::ConvertiFichier()
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
}*/
