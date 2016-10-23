#include "Decodeur.h"

Decodeur::Decodeur(){ }

Decodeur::~Decodeur()
{
    device->stopDepth();
    device->stopVideo();
    DecodeurEnMarche = false;
}

void Decodeur::Init(MyFreenectDevice& freenectSingleton)
{
    device = &freenectSingleton;
    device->startVideo();
    device->startDepth();
    pthread_t convertisseur;
    pthread_create(&convertisseur, NULL, Convertir, this);
}

void Decodeur::UpdateFPS(bool showFpsConsole)
{
    if(updateFPS)
    {
        FPSStarTime = std::clock();
        frame = 0;
        nbEchantillonsParSecond = 0;
        updateFPS = false;
    }
    else
    {
        frame++;
        clock_t now = std::clock();
        if(now - 1000000 >= FPSStarTime)
        {
            updateFPS = true;

            if(showFpsConsole)
            {
                std::cout << "fps: " << frame << " nombre d'echantillons par seconde : " << nbEchantillonsParSecond << std::endl;
            }
        }
    }
}

void Decodeur::UpdateCloudOfPoint()
{
    device->getRGB(rgb);
    if(device->getDepth(depth))
    {
        ++nbEchantillonsParSecond;
    }
    if(updateCloud)
    {
        std::cout << "Echantillonnage!!!!" << std::endl;
        std::vector<Vector3> snapshot = std::vector<Vector3>();
        float HauteurMax = std::stof(Config::Instance().GetString("HAUTEUR_MAX"));
        float HauteurMin = std::stof(Config::Instance().GetString("HAUTEUR_MIN"));
        float HauteurKin = std::stof(Config::Instance().GetString("HAUTEUR_KINECT"));
        //transforme les donnees du buffer en coordonne monde
        float f = 595.f;
        for(int i = 0; i < IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y; ++i)
        {
            Vector3 vec = Vector3((i%IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_X-1)/2.f) * depth[i] / f,
                                  (-(i/IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_Y-1)/2.f) * depth[i] / f) + HauteurKin,
                                  -depth[i]);

            if(vec.y > HauteurMax || vec.y < HauteurMin )
            {
                continue;
            };
            snapshot.push_back(RealCam.matrixToWorld * vec);

        }


        CloudSamplingTime = std::clock();
        updateCloud = false;

        cloudBuffer.Insert(rgb, snapshot);
    }
    else
    {
        clock_t now = std::clock();
        if(now - nextSampling * 1000 >= CloudSamplingTime)
        {
            //updateCloud = true;
        }
    }
}

void Decodeur::SaveCarte()
{
    Environnement.SaveCarte();
}

void Decodeur::RunLoop()
{
    device->setTiltDegrees(0.0);
    UpdateFPS(true);
    UpdateCloudOfPoint();
}

void* Decodeur::Convertir(void* parent)
{
    Decodeur* decodeur = (Decodeur*)parent;
    std::vector<Vector3> points = std::vector<Vector3>();
    int indiceTraite = -1;
    while(decodeur->DecodeurEnMarche)
    {
        indiceTraite = decodeur->cloudBuffer.GetCopyCloudPointToConvert(points);
        if(indiceTraite != -1)
        {
            //algorithme de regression lineaire

            decodeur->cloudBuffer.Converted[indiceTraite] = true;
        }
    }
}
