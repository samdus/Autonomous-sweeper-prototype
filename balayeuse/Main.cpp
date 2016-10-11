//#include <cstdlib>
//#include <ctime>
//#include <iostream>
//#include <vector>
//#include "MyFreenectDevice.h"
//#include <math.h>
//#include "SceneCamera.h"
//#include "CloudPointContainer.h"
//
//#define CLOUD_POINT_SAMPLING_FREQUENCY 500 //millisecond
//
//Freenect::Freenect freenect;
//MyFreenectDevice* device;
//CloudPointContainer cloudBuffer = CloudPointContainer();
//
//int nbEchantillonsParSecond = 0;
//bool updateFPS = true;
//clock_t FPSStarTime = 0;
//
//bool updateCloud = true;
//clock_t CloudSamplingTime = 0;
//
//std::vector<uint16_t> depth(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y);
//
//void UpdateFPS(bool showFpsConsole, bool showFpsInScene)
//{
//    if(updateFPS)
//    {
//        FPSStarTime = std::clock();
//        nbEchantillonsParSecond = 0;
//        updateFPS = false;
//    }
//    else
//    {
//        clock_t now = std::clock();
//        if(now - 1000000 >= FPSStarTime)
//        {
//            updateFPS = true;
//
//            if(showFpsConsole)
//            {
//                std::cout << "nombre d'echantillons par seconde : " << nbEchantillonsParSecond << std::endl;
//            }
//        }
//    }
//}
//
//void UpdateCloudOfPoint()
//{
//    if(device->getDepth(depth))
//    {
//        ++nbEchantillonsParSecond;
//    }
//    if(updateCloud)
//    {
//        CloudSamplingTime = std::clock();
//        updateCloud = false;
//        cloudBuffer.InsertDepth(depth);
//    }
//    else
//    {
//        clock_t now = std::clock();
//        if(now - CLOUD_POINT_SAMPLING_FREQUENCY * 1000 >= CloudSamplingTime)
//        {
//            updateCloud = true;
//        }
//    }
//}
//
//void printInfo()
//{
//    std::cout << "\nAvailable Controls:"              << std::endl;
//    std::cout << "==================="                << std::endl;
//    std::cout << "Quit         :   Ctrl + C\n"        << std::endl;
//}
//
//int main(int argc, char **argv)
//{
//    device = &freenect.createDevice<MyFreenectDevice>(0);
//    device->startDepth();
//
//    printInfo();
//
//    while(1)
//    {
//        UpdateFPS(true,false);
//        UpdateCloudOfPoint();
//    }
//
//    return 0;
//}
