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
                std::cout << "fps: " << frame << std::endl;
                std::cout << "nombre d'echantillons par seconde : " << nbEchantillonsParSecond << std::endl;
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
        RealCam.matrixToWorld = RealCam.matrixToWorld.translate(50,0,0);
        CloudSamplingTime = std::clock();
        updateCloud = false;
        cloudBuffer.Insert(rgb, depth);
    }
    else
    {
        clock_t now = std::clock();
        if(now - CLOUD_POINT_SAMPLING_FREQUENCY * 1000 >= CloudSamplingTime)
        {
            updateCloud = true;
        }
    }
}

void Decodeur::RunLoop()
{
    UpdateFPS(true);
    UpdateCloudOfPoint();
}
