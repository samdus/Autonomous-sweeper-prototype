#include <iostream>
#include <vector>
#include "MyFreenectDevice.h"
#include "SceneCamera.h"
#include "CloudPointContainer.h"
#include "Matrices.h"
#include "Config.h"

#define CLOUD_POINT_SAMPLING_FREQUENCY 5000 //millisecond

class Decodeur
{
    //private:

    public:
    MyFreenectDevice* device;
    CloudPointContainer cloudBuffer = CloudPointContainer();

    bool color = true;                      // Flag to indicate to use of color in the cloud
    SceneCamera RealCam = SceneCamera();    // Camera to simulate the kinect in the real world

    int frame = 0;
    int nbEchantillonsParSecond = 0;
    clock_t FPSStarTime = 0;
    bool updateFPS = true;
    float fps = 0.0;

    bool updateCloud = false;
    clock_t CloudSamplingTime = 0;

    std::vector<uint8_t> rgb = std::vector<uint8_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y*3);
    std::vector<uint16_t> depth = std::vector<uint16_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y);
    std::vector<Vector3> depthWorld = std::vector<Vector3>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y);

    Decodeur();
    ~Decodeur();

    void Init(MyFreenectDevice& freenectSingleton);
    void UpdateFPS(bool showFpsConsole);

    void UpdateCloudOfPoint();

    void RunLoop();
};
