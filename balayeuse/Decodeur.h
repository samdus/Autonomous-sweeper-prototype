
#ifndef DECODEURH
#define DECODEURH
#include <iostream>
#include <vector>
#include "MyFreenectDevice.h"
#include "SceneCamera.h"
#include "CloudPointContainer.h"
#include "Matrices.h"
#include "Config.h"
#include "Convertisseur.h"
#include "../Robot/CommunicatorArduino/ArduinoCommunicator.h"
#include <stdexcept>
#include "../mongoWrapper/MongoWrapper.cpp"
#include <string>
enum ActionEnum{
    Avancer, Tourner
};

struct Action
{
    ActionEnum action;
    int valeur;
    Action(ActionEnum act, int val)
    {
        action = act;
        valeur = val;
    }
};

class Decodeur
{
    private:
    int nextSampling = 500; //millisecond
    int frame = 0;
    int nbEchantillonsParSecond = 0;
    size_t quantiteDeSegmentEnvironnement = 0;
    clock_t FPSStarTime = 0;
    bool updateFPS = true;
    float fps = 0.0;
    clock_t CloudSamplingTime = 0;
    clock_t KinectInitTime = 0;
    clock_t ArduinoInitTime = 0;
    MyFreenectDevice* device;
    ArduinoCommunicator arduinoCommunicator;
    float HauteurMax = 0.0;
    float HauteurMin = 0.0;
    float HauteurKin = 0.0;
    float OffsetKin = 0.0;
    float DistanceMax = 0.0;
    float DistanceMin = 0.0;
    bool MultithreadActiver = false; //utiliser en debug probablement pas necessaire
    bool DebugConsole = false;
    bool DebugServeur = false;
    bool KinectAccessible = false;
    bool KinectCameraActiver = false;
    bool ArduinoAccessible = false;
    bool ModeAutomatique = true;
    bool EnMarche = true;
    std::vector<Action> actions;

    void InitKinect();
    void InitCommunicationArduino();
    void InitCommunicationServeur();
    void InitConfiguration();
    void UpdateCommande();
    void ExecuteCommande();
    void PrendreEchantillonEnvironnement();


    public:
    Convertisseur convertisseur;
    CloudPointContainer cloudBuffer = CloudPointContainer();
    SceneCamera RealCam = SceneCamera();    // Camera to simulate the kinect in the real world
    bool updateCloud = false;
    std::vector<uint8_t> rgb = std::vector<uint8_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y*3);
    std::vector<uint16_t> depth = std::vector<uint16_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y);

    Decodeur();
    ~Decodeur();

    void Init();
    void EnvoieDebug(std::string message, std::string categorie);
    void UpdateFPS();

    void UpdateCloudOfPoint();

    bool RunLoop();
};

#endif /* !DECODEURH */
