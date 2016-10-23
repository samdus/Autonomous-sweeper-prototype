#include <iostream>
#include <vector>
#include "MyFreenectDevice.h"
#include "SceneCamera.h"
#include "CloudPointContainer.h"
#include "Matrices.h"
#include "Config.h"
#include "Carte.h"

class Decodeur
{
    private:
    int nextSampling = 5000; //millisecond
    int frame = 0;
    int nbEchantillonsParSecond = 0;
    clock_t FPSStarTime = 0;
    bool updateFPS = true;
    float fps = 0.0;
    clock_t CloudSamplingTime = 0;
    MyFreenectDevice* device;

    public:
    Carte Environnement;
    CloudPointContainer cloudBuffer = CloudPointContainer();
    SceneCamera RealCam = SceneCamera();    // Camera to simulate the kinect in the real world
    bool updateCloud = false;
    bool DecodeurEnMarche = true;
    std::vector<uint8_t> rgb = std::vector<uint8_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y*3);
    std::vector<uint16_t> depth = std::vector<uint16_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y);

    Decodeur();
    ~Decodeur();

    void Init(MyFreenectDevice& freenectSingleton);
    void UpdateFPS(bool showFpsConsole);

    void UpdateCloudOfPoint();
    void SaveCarte();
    static void* Convertir(void* decodeur);

    void RunLoop();
};
