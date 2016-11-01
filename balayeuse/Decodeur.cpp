#include "Decodeur.h"

Decodeur::Decodeur(){ }

Decodeur::~Decodeur()
{
    device->stopDepth();
    device->stopVideo();
    convertisseur.ContinuerConvertion = false;
}

void Decodeur::Init(MyFreenectDevice& freenectSingleton)
{
    device = &freenectSingleton;
    device->startVideo();
    device->startDepth();
    convertisseur.DemarreThread(&cloudBuffer);
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
        float OffsetKin = std::stof(Config::Instance().GetString("OFFSET_KINECT"));
        float DistanceMax = std::stof(Config::Instance().GetString("FAR_CLIPPING")) + OffsetKin;
        float DistanceMin = std::stof(Config::Instance().GetString("NEAR_CLIPPING")) + OffsetKin;
        //transforme les donnees du buffer en coordonne monde


        float f = 595.f;
        for(int i = 0; i < IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y; ++i)
        {
            int j = i%IR_CAMERA_RESOLUTION_Y * IR_CAMERA_RESOLUTION_X + i/IR_CAMERA_RESOLUTION_Y;
            Vector3 vec = Vector3((j%IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_X-1)/2.f) * depth[j] / f,
                                  (-(j/IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_Y-1)/2.f) * depth[j] / f) + HauteurKin,
                                  -depth[j]);

            if(vec.y > HauteurMax || vec.y < HauteurMin || vec.z < -DistanceMax || vec.z > -DistanceMin )
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

void Decodeur::RunLoop()
{
    device->setTiltDegrees(0.0);
    UpdateFPS(false);
    UpdateCloudOfPoint();
}
