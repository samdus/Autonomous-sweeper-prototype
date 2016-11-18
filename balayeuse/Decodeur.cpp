#include "Decodeur.h"

Freenect::Freenect freenect;

Decodeur::Decodeur(){ }

Decodeur::~Decodeur()
{
    if(KinectAccessible)
    {
        device->stopDepth();
        device->stopVideo();
    }
    convertisseur.ContinuerConvertion = false;
}

void Decodeur::InitKinect()
{
    KinectInitTime = std::clock();
    try
    {
        device = &freenect.createDevice<MyFreenectDevice>(0);
        KinectAccessible = true;
        device->startVideo();
        device->startDepth();
        std::cout << "\nKinect demarree!" << std::endl;
    }
    catch(std::runtime_error e)
    {
        std::cout << "Impossible de demarrer la Kinect" << std::endl;
    }
}

void Decodeur::InitCommunicationArduino()
{
}

void Decodeur::InitCommunicationServeur()
{
}

void Decodeur::InitConfiguration()
{
    HauteurMax = std::stof(Config::Instance().GetString("HAUTEUR_MAX"));
    HauteurMin = std::stof(Config::Instance().GetString("HAUTEUR_MIN"));
    HauteurKin = std::stof(Config::Instance().GetString("HAUTEUR_KINECT"));
    OffsetKin = std::stof(Config::Instance().GetString("OFFSET_KINECT"));
    DistanceMax = std::stof(Config::Instance().GetString("FAR_CLIPPING")) + OffsetKin;
    DistanceMin = std::stof(Config::Instance().GetString("NEAR_CLIPPING")) + OffsetKin;
    MultithreadActiver = std::stoi(Config::Instance().GetString("MULTITHREAD")) == 1;
}

void Decodeur::Init()
{
    InitKinect();
    InitCommunicationArduino();
    InitCommunicationServeur();
    InitConfiguration();

    if(MultithreadActiver)
    {
        convertisseur.DemarreThread(&cloudBuffer);
    }
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
                std::cout << "fps: " << frame << " nombre d'echantillons par seconde : " << nbEchantillonsParSecond << " next sampling " << nextSampling << std::endl;
            }
        }
    }
}

void Decodeur::UpdateCloudOfPoint()
{
    device->getRGB(rgb);
    device->getDepth(depth);
    if(updateCloud)
    {
        ++nbEchantillonsParSecond;
        //std::cout << "Echantillonnage!!!!" << std::endl;
        std::vector<Vector3> snapshot = std::vector<Vector3>();
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

        if(!cloudBuffer.Insert(rgb, snapshot))
        {
            nextSampling += nextSampling / 2;//slow down the sampling
        }
    }
    else
    {
        clock_t now = std::clock();
        if(now - nextSampling * 1000 >= CloudSamplingTime)
        {
            //updateCloud = true;
            nextSampling = std::max(nextSampling - nextSampling / CLOUD_POINT_CIRCULAR_BUFFER, (1000/30));//inutile d'être en dessous de 30 image seconde
        }
    }
}

void Decodeur::RunLoop()
{
    if(!KinectAccessible)
    {
        if(KinectInitTime < std::clock() - 5000000)
        {
            InitKinect();
        }
        return;
    }
    if(ArduinoAccessible && !ModeAutomatique)
    {
        //aller chercher les commandes et les executer
    }

    if(ArduinoAccessible && ModeAutomatique)
    {
        //deplacement autonome
    }
    else
    {
        try
        {
            device->setTiltDegrees(0.0);
            UpdateFPS(false);
            UpdateCloudOfPoint();
            if(!MultithreadActiver)
            {
                convertisseur.Convertir(cloudBuffer);
            }
        }
        catch(std::runtime_error e)
        {
            freenect.deleteDevice(0);
            std::cout << "\nError: Kinect deconnectee !" << std::endl;
            KinectAccessible = false;
        }
    }
}
