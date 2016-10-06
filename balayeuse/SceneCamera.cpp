#pragma once

#include "SceneCamera.h"

SceneCamera::SceneCamera(){}
SceneCamera::~SceneCamera(){}

void SceneCamera::Update()
{
    if(deltaMove)
    {
        posX += deltaMove * lX * 100.0;
        posY += deltaMove * lY * 100.0;
        posZ += deltaMove * lZ * 100.0;
    }
    if(deltaStrafe)
    {
        posX += deltaStrafe * -lZ * 100.0;
        posZ += deltaStrafe * lX * 100.0;
    }
}

void SceneCamera::DebugInfo()
{
    std::cout << "position " << posX << " " << posY << " " << posZ << std::endl
              << "angleX " << lX << "angleY " << lY << "angleZ " << lZ << std::endl
              << "upVector " << upX << " " << upY << " " << upZ << std::endl;
}
