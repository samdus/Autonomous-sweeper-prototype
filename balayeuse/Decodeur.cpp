#include "Decodeur.h"

Freenect::Freenect freenect;
MongoWrapper serveur;
Config ConfigHelper;

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
    if(DebugConsole || DebugServeur)
    {
        std::string message = "test de connection avec le serveur";
        if(DebugConsole)
            std::cout << message;
        if(DebugServeur)
            serveur.writeConsole(message , "error");
    }
}

void Decodeur::InitConfiguration()
{
    HauteurMax = std::stof(ConfigHelper.GetString("HAUTEUR_MAX"));
    HauteurMin = std::stof(ConfigHelper.GetString("HAUTEUR_MIN"));
    HauteurKin = std::stof(ConfigHelper.GetString("HAUTEUR_KINECT"));
    OffsetKin = std::stof(ConfigHelper.GetString("OFFSET_KINECT"));
    DistanceMax = std::stof(ConfigHelper.GetString("FAR_CLIPPING")) + OffsetKin;
    DistanceMin = std::stof(ConfigHelper.GetString("NEAR_CLIPPING")) + OffsetKin;
    MultithreadActiver = std::stoi(ConfigHelper.GetString("MULTITHREAD")) == 1;
    DebugConsole = std::stoi(ConfigHelper.GetString("DEBUG_MESSAGE_CONSOLE")) == 1;
    DebugServeur = std::stoi(ConfigHelper.GetString("DEBUG_MESSAGE_SERVEUR")) == 1;
}

void Decodeur::Init()
{
    convertisseur.InitialisationConfig(ConfigHelper);
    InitKinect();
    InitCommunicationArduino();
    InitCommunicationServeur();
    InitConfiguration();

    if(MultithreadActiver)
    {
        convertisseur.DemarreThread(&cloudBuffer);
    }
}

void Decodeur::UpdateFPS()
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

            if(DebugConsole || DebugServeur)
            {
                std::string message = "fps: " + std::to_string(frame) + " nombre d'echantillons par seconde : " + std::to_string(nbEchantillonsParSecond) + " next sampling " + std::to_string(nextSampling) + "\n";
                if(DebugConsole)
                    std::cout << message;
                if(DebugServeur)
                    serveur.writeConsole(message, "info");
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
        for(size_t i = 0; i < IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y; ++i)
        {
            size_t j = i%IR_CAMERA_RESOLUTION_Y * IR_CAMERA_RESOLUTION_X + i/IR_CAMERA_RESOLUTION_Y;
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
            updateCloud = true;
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
            UpdateFPS();
            UpdateCloudOfPoint();
            if(!MultithreadActiver)
            {
                convertisseur.Convertir(cloudBuffer);
            }
        }
        catch(std::runtime_error e)
        {
            freenect.deleteDevice(0);

            if(DebugConsole || DebugServeur)
            {
                string message = "\nError: Kinect deconnectee !\n";
                if(DebugConsole)
                    std::cout << message;
                if(DebugServeur)
                    serveur.writeConsole(message, "error");
            }
            KinectAccessible = false;
        }
    }
}
