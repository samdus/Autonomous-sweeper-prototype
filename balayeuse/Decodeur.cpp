#include "Decodeur.h"

Freenect::Freenect freenect;
MongoWrapper serveur;
Config ConfigHelper;
int forcedStops = 0;
int incAffichage =0;
Decodeur::Decodeur(){ }

Decodeur::~Decodeur()
{
    serveur.addUpdate("working", false);
    if(KinectAccessible)
    {
        device->stopDepth();
        if(KinectCameraActiver)
        {
            device->stopVideo();
        }
    }
    convertisseur.Environnement.SaveCarte();
    convertisseur.ContinuerConvertion = false;
}

void afficherDebug(int16_t debug[4])
{
	if(incAffichage > 200){
        switch (debug[0])
        {
            case ArduinoCommunicator::Fonction::InfoDistanceObjet:
                serveur.addUpdate("distanceSonar", debug[1]);
                //cout << "Distance: " << debug[1] << endl;
                break;
            case ArduinoCommunicator::Fonction::InfoOrientation:
                //cout << "Orientation: " << debug[1] << endl;
                break;
            case ArduinoCommunicator::Fonction::InfoVitesseMoteur:
                if(debug[1] == 0){
                    serveur.addUpdate("leftWheel", debug[2]);
                    //cout << "Vitesse du moteur gauche: " << debug[2] << endl;
                }else{
                    serveur.addUpdate("rightWheel", debug[2]); // voir avec sam
                    //cout << "Vitesse du moteur droit: " << debug[2] << endl;
                }
                break;
        }
        if(incAffichage > 204){
            incAffichage=0;
        }
    }else{
        incAffichage = incAffichage+1;
    }


	switch (debug[0])
	{
	case ArduinoCommunicator::Fonction::DirectionChoisie:
		//cout << debug[1];
		//EnvoiWeb += std::to_string(debug[1]);
		break;
	case ArduinoCommunicator::Fonction::Erreur:
		switch (debug[1])
		{
		case ArduinoCommunicator::TypeErreur::Obstacle:
            forcedStops++;
            serveur.addUpdate("forcedStop", forcedStops); // voir avec sam
			//cout << "Obstacle!!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::FonctionInconnue:
			//cout << "Fonction inconnue!!" << endl;
            serveur.writeConsole("Erreur de fonction du Arduinno'", "error");
			break;
		case ArduinoCommunicator::TypeErreur::IO:
            serveur.writeConsole("Erreur d'entrée sortie du Arduinno'", "error");
			//cout << "Erreur de IO!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::ErreurInitialisation:
            serveur.writeConsole("Erreur d'initialization du arduino", "error");
			//cout << "Erreur d'initialisation!" << endl;
			break;
		case ArduinoCommunicator::TypeErreur::EntreeInconnue:
            serveur.writeConsole("Erreur inconnue du arduino", "error");
			//std::bitset<sizeof(int16_t) * 8> binaire(debug[2]);
			//cout << "Entree inconnue: " << endl << binaire << endl;
			break;
		}
		break;
	}
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
        EnvoieDebug(std::string("Kinect demarree!\n"), std::string("info"));
    }
    catch(std::runtime_error e)
    {
        EnvoieDebug("Impossible de demarrer la Kinect\n", "error");
    }
}

void Decodeur::InitCommunicationArduino()
{
    ArduinoInitTime = std::clock();
    ArduinoAccessible = arduinoCommunicator.init(afficherDebug);
    if(!ArduinoAccessible)
    {
        EnvoieDebug("Initialisation d'Arduino impossible\n", "error");
    }
    else
    {
        EnvoieDebug("Initialisation d'Arduino reussi\n", "error");
    }
}

void Decodeur::InitCommunicationServeur()
{
    //if(!testdeconnection)
    //EnvoieDebug("test de connection avec le serveur\n", "info");
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
    //Consomme toute les commandes
    MongoCommand* commande = serveur.getCommand();
    bool ignore = false;
    float dotprod, cote, angleCourant = 0.0;
    while(commande != NULL)
    {
        switch(commande->m_commandInfo.thecommand)
        {
            case MANUAL:
                std::cout << "test MANUAL\n";
                if(ModeAutomatique)
                {
                    actions.clear();
                }
            break;
            case GOTO:
                std::cout << "test GOTO\n";
                ignore = false;
                for(Action action : actions)
                {
                    if(action.action == Avancer)
                    {
                        ignore = true;
                        break;
                    }
                }
                if(!ignore)
                {
                    //calculer la rotation a faire et le deplacement ensuite
                    Vector2 directionObjectif = Vector2(-commande->m_commandInfo.x - RealCam.position.x, commande->m_commandInfo.y - RealCam.position.z).normalize();
                    dotprod = Vector2(RealCam.lX, RealCam.lZ).dot(directionObjectif);
                    cote = Vector2(RealCam.lX, RealCam.lZ).dot(Vector2(-directionObjectif.y, directionObjectif.x));//negatif = a gauche sinon a droite

                    if(dotprod != 0.0)
                    {
                        angleCourant = acos(dotprod) * RAD2DEG;
                        if(cote < 0.0)
                        {
                            angleCourant = -angleCourant;
                        }
                        actions.push_back(Action(TournerDeXDegree, angleCourant));
                    }

                    actions.push_back(Action(Avancer, Vector2(RealCam.position.x, RealCam.position.z).distance(Vector2(commande->m_commandInfo.x, commande->m_commandInfo.y))));
                }
                else
                {
                    std::cout << "Impossible d'ajouter une autre déplacement\n";
                }
            break;
            case CLOSE:
                std::cout << "test close\n";
                convertisseur.ContinuerConvertion = false;
                EnMarche = false;
            break;
            case STARTDEBUG:
                std::cout << "test STARTDEBUG\n";
                DebugServeur = true;
            break;
            case STOPDEBUG:
                std::cout << "test STOPDEBUG\n";
                DebugServeur = false;
            break;
            case SCAN:
                std::cout << "test SCAN\n";
                actions.push_back(Action(TournerDeXDegree, 45));
            break;
            case TURN:
                std::cout << "test TURN\n";
                actions.push_back(Action(TournerDeXDegree, commande->m_commandInfo.x));
            break;
            case AUTOMATIC:
                std::cout << "test AUTOMATIC\n";
                if(!ModeAutomatique)
                {
                    actions.clear();
                }
            break;
            case TAKEPHOTO:
                std::cout << "test TAKEPHOTO\n";
            break;
             case INVALID:
                std::cout << "test INVALID\n";
                serveur.writeConsole("Erreur d'interprétation de commande sur le robot", "error");
            break;
            default:
                std::cout << "invalid\n";
            break;
        };

        commande = serveur.getCommand();
    }
}

void Decodeur::ExecuteActions()
{
    if(actions.size() != lastPollingValue){
         serveur.addUpdate("nbCommands", (int) actions.size());
         lastPollingValue = actions.size();
    }
    if(actions.size() != 0)
    {
        if(!ArduinoAccessible)
        {
            return;
        }
        std::cout << "execute actions\n";
        switch(actions[0].action)
        {
        case Avancer:
            std::cout << "J'avance de " << actions[0].valeur << "mm" << std::endl;
            arduinoCommunicator.avancePendantXDixiemeSec(actions[0].valeur);
            RealCam.Avance(actions[0].valeur);
            PrendreEchantillonEnvironnement();
            break;
        case TournerDeXDegree:
            std::cout << "Je troune de " << actions[0].valeur << std::endl;
            if(actions[0].valeur > 0.0)
            {
                arduinoCommunicator.tourneDroitePendant((int16_t)actions[0].valeur);
            }
            else
            {
                arduinoCommunicator.tourneGauchePendant((int16_t)(actions[0].valeur));
            }

            RealCam.RotateY(-actions[0].valeur);//negative since the cam is upside down
            PrendreEchantillonEnvironnement();
            break;
        }

        actions.erase(actions.begin(),actions.begin() + 1);
    }
    else//s'il n'a rien a faire continue de prendre des echantillons de son environnement
    {
        PrendreEchantillonEnvironnement();
    }
}

void Decodeur::PrendreEchantillonEnvironnement()
{
    try
    {
        device->setTiltDegrees(0.0);
        while(!UpdateCloudOfPoint());//doit prendre un echantillon
        //UpdateCloudOfPoint();
        if(!MultithreadActiver)
        {
            convertisseur.Convertir(cloudBuffer);
        }
    }
    catch(std::runtime_error e)
    {
        freenect.deleteDevice(0);
        KinectAccessible = false;

        EnvoieDebug("Error: lors de la prise d'echantillon\n", "error");
    }
}

void Decodeur::Init()
{
    EnvoieDebug("Demarrage", "info");
    InitConfiguration();
    convertisseur.InitialisationConfig(ConfigHelper);
    InitKinect();
    InitCommunicationArduino();
    InitCommunicationServeur();
    quantiteDeSegmentEnvironnement = convertisseur.Environnement.GetSegments().size();
    this->mapIter++;
    serveur.addUpdate("working", true);
    serveur.addUpdate("mapIter", this->mapIter);
    serveur.writeMap(convertisseur.Environnement.GetSegments(), (int)RealCam.position.x,(int)RealCam.position.z);
    if(MultithreadActiver)
    {
        convertisseur.DemarreThread(&cloudBuffer);
    }
}

void Decodeur::EnvoieDebug(std::string message, std::string categorie)
{
    if(DebugConsole || DebugServeur)
    {
        if(DebugConsole)
            std::cout << message;
        if(DebugServeur)
            serveur.writeConsole(message, categorie);
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
        if(now - 10000000 >= FPSStarTime)
        {
            updateFPS = true;
            serveur.addUpdate("kinectPolling", (float)(nbEchantillonsParSecond/10.0));
            //EnvoieDebug("fps: " + std::to_string(frame) + " nombre d'echantillons par seconde : " + std::to_string(nbEchantillonsParSecond) + " next sampling " + std::to_string(nextSampling) + "\n", "info");
        }
    }
}

bool Decodeur::UpdateCloudOfPoint()
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
        return true;
    }
    else
    {
        clock_t now = std::clock();
        if(now - nextSampling * 1000 >= CloudSamplingTime)
        {
            updateCloud = true;
            nextSampling = std::max(nextSampling - nextSampling / CLOUD_POINT_CIRCULAR_BUFFER, (1/15));//inutile de prendre plus de 30 image seconde
        }
    }
    return false;
}

void Decodeur::EnvoieCarteAuServeur()
{
    if(CarteServeurAJour && quantiteDeSegmentEnvironnement != convertisseur.Environnement.GetSegments().size())//difference trouvee
    {
        CarteServeurAJour = false;
    }

    if(!CarteServeurAJour && EnvoieDeLaCarteTime < std::clock() - 5000000)
    {
        CarteServeurAJour = true;
        EnvoieDeLaCarteTime = std::clock();
        EnvoieDebug("Envoie de la carte au serveur\n", "info");
        quantiteDeSegmentEnvironnement = convertisseur.Environnement.GetSegments().size();
        serveur.writeMap(convertisseur.Environnement.GetSegments(), (int)RealCam.position.x,(int)RealCam.position.z);
    }
}

bool Decodeur::RunLoop()
{
    if(!EnMarche)
    {
        return false;
    }

    UpdateCommande();

    if(!KinectAccessible)
    {
        if(KinectInitTime < std::clock() - 10000000)
        {
            InitKinect();
        }
        return true;
    }

    if(!ArduinoAccessible || !arduinoCommunicator.isLectureEnFonction())
    {
        ArduinoAccessible = false;
        if(ArduinoInitTime < std::clock() - 10000000)
        {
            InitCommunicationArduino();
        }
    }

    try
    {
        device->setTiltDegrees(0.0);
    }
    catch(std::runtime_error e)
    {
        freenect.deleteDevice(0);
        KinectAccessible = false;

        EnvoieDebug("Error: lors de la prise de la stabilisation de la kinect\n", "error");
        return true;
    }

    EnvoieCarteAuServeur();
    UpdateFPS();
    ExecuteActions();

    return true;
}
