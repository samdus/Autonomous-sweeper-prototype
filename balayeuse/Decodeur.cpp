#include "Decodeur.h"

Decodeur::Decodeur(){ }

Decodeur::~Decodeur()
{
    device->stopDepth();
    device->stopVideo();
}

void Decodeur::Init(MyFreenectDevice& freenectSingleton)
{
    device = &freenectSingleton;
    device->startVideo();
    device->startDepth();
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
        std::cout << "Sauvegarde du derniere echantillon!!!!" << std::endl;

        //transforme les donnees du buffer en coordonne monde
        float f = 595.f;
        for(int i = 0; i < IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y; ++i)
        {
            Vector3 vec = Vector3((i%IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_X-1)/2.f) * depth[i] / f,
                                  (-(i/IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_Y-1)/2.f) * depth[i] / f) - std::stof(Config::Instance().GetString("HAUTEUR_KINECT")),
                                  -depth[i]);

            depthWorld[i] = RealCam.matrixToWorld * vec;
        }

        CloudSamplingTime = std::clock();
        updateCloud = false;

        cloudBuffer.Insert(rgb, depthWorld);
    }
    else
    {
        clock_t now = std::clock();
        if(now - CLOUD_POINT_SAMPLING_FREQUENCY * 1000 >= CloudSamplingTime)
        {
            //updateCloud = true;
        }
    }
}

void Decodeur::RunLoop()
{
    UpdateFPS(true);
    UpdateCloudOfPoint();
}
