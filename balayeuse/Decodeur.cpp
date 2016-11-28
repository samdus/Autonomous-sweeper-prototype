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
        if(KinectCameraActiver)
        {
            device->stopVideo();
        }
    }
    convertisseur.ContinuerConvertion = false;
}

void afficherDebug(int16_t debug[4])
{
	/*switch (debug[0])
	{
	case ArduinoCommunicator::Fonction::InfoDistanceObjet:
		cout << "Distance: " << debug[1] << endl;
		break;
	case ArduinoCommunicator::Fonction::InfoOrientation:
		cout << "Orientation: " << debug[1] << endl;
		break;
	case ArduinoCommunicator::Fonction::InfoVitesseMoteur:
		if(debug[1] == 0)
			cout << "Vitesse du moteur gauche: " << debug[2] << endl;
		else
			cout << "Vitesse du moteur droit: " << debug[2] << endl;
		break;
	case ArduinoCommunicator::Fonction::DirectionChoisie:
		cout << debug[1];
		EnvoiWeb += std::to_string(debug[1]);
		break;
	case ArduinoCommunicator::Fonction::Erreur:
		switch (debug[1])
		{
		case ArduinoCommunicator::TypeErreur::Obstacle:
			cout << "Obstacle!!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::FonctionInconnue:
			cout << "Fonction inconnue!!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::IO:
			cout << "Erreur de IO!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::ErreurInitialisation:
			cout << "Erreur d'initialisation!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::EntreeInconnue:
			std::bitset<sizeof(int16_t) * 8> binaire(debug[2]);
			cout << "Entree inconnue: " << endl << binaire << endl;
			break;
		}
		break;
	}*/
}

void Decodeur::InitKinect()
{
    KinectInitTime = std::clock();
    try
    {
        device = &freenect.createDevice<MyFreenectDevice>(0);
        KinectAccessible = true;

        device->startDepth();
        if(KinectCameraActiver)
        {
            device->startVideo();
        }
        updateCloud = true;
        std::cout << "\nKinect demarree!" << std::endl;
    }
    catch(std::runtime_error e)
    {
        if(DebugConsole || DebugServeur)
        {
            std::string message = "Impossible de demarrer la Kinect";
            if(DebugConsole)
                std::cout << message;
            if(DebugServeur)
                serveur.writeConsole(message , "error");
        }
    }
}

void Decodeur::InitCommunicationArduino()
{
    ArduinoInitTime = std::clock();
    ArduinoAccessible = arduinoCommunicator.init(afficherDebug);
    if(!ArduinoAccessible && (DebugConsole || DebugServeur))
    {
        std::string message = "Initialisation d'Arduino impossible";
        if(DebugConsole)
            std::cout << message;
        if(DebugServeur)
            serveur.writeConsole(message , "error");
    }
}

void Decodeur::InitCommunicationServeur()
{
    if(/* !testdeconnection ||*/ DebugConsole || DebugServeur)
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
    KinectCameraActiver = std::stoi(ConfigHelper.GetString("CAMERA_COULEUR")) == 1;
}

void Decodeur::UpdateCommande()
{
    if(ListeDeCommandes.size() == 0)
    {
        ListeDeCommandes.push_back(MongoCommand().m_commandInfo);
    }
}

void Decodeur::ExecuteCommande()
{
    if(ListeDeCommandes.size() != 0)
    {
        arduinoCommunicator.tourneAuDegresX(ListeDeCommandes[0].x);
        RealCam.RotateY(ListeDeCommandes[0].x);

        /*switch(ListeDeCommandes[0].command)
        {
            case "scan":
            break;
        }*/
    }
}

void Decodeur::PrendreEchantillonEnvironnement()
{
    try
    {
        device->setTiltDegrees(0.0);
        UpdateCloudOfPoint();
        if(!MultithreadActiver)
        {
            convertisseur.Convertir(cloudBuffer);
        }
    }
    catch(std::runtime_error e)
    {
        freenect.deleteDevice(0);
        KinectAccessible = false;

        if(DebugConsole || DebugServeur)
        {
            std::string message = "Error: lors de la loop principale\n";
            if(DebugConsole)
                std::cout << message;
            if(DebugServeur)
                serveur.writeConsole(message, "error");
        }
    }
}

void Decodeur::Init()
{
    serveur.writeConsole("Demarrage" , "info");
    InitConfiguration();
    convertisseur.InitialisationConfig(ConfigHelper);
    InitKinect();
    InitCommunicationArduino();
    InitCommunicationServeur();

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
    if(KinectCameraActiver /*&& Pret a l'envoie de photo au serveur*/)
    {
        device->getRGB(rgb);
    }
    if(updateCloud && device->getDepth(depth))
    {
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

        if(!cloudBuffer.InsertDepth(snapshot))
        {
            nextSampling += nextSampling / 2;//slow down the sampling
        }
        else
        {
            ++nbEchantillonsParSecond;
        }
    }
    else
    {
        clock_t now = std::clock();
        if(now - nextSampling * 1000 >= CloudSamplingTime)
        {
            updateCloud = true;
            nextSampling = std::max(nextSampling - nextSampling / CLOUD_POINT_CIRCULAR_BUFFER, (1000/2));//inutile de prendre plus de 2 image seconde
        }
    }
}

void Decodeur::RunLoop()
{
    if(!KinectAccessible)
    {
        if(KinectInitTime < std::clock() - 10000000)
        {
            InitKinect();
        }
        return;
    }

    if(!ArduinoAccessible || !arduinoCommunicator.isLectureEnFonction())
    {
        ArduinoAccessible = false;
        if(ArduinoInitTime < std::clock() - 10000000)
        {
            InitCommunicationArduino();
        }
        return;
    }

    UpdateFPS();
    UpdateCommande();
    ExecuteCommande();
}
