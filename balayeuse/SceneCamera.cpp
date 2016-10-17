#pragma once

#include "SceneCamera.h"

SceneCamera::SceneCamera()
{
    position = Vector3(0.0,  0.0, 0.0);
    upVector = Vector3(0.0, -1.0, 0.0);
    matrixToWorld = Matrix4();
}
SceneCamera::~SceneCamera(){}

void SceneCamera::Update()
{
    if(deltaMove)
    {
        position.x += deltaMove * lX * 100.0;
        position.y += deltaMove * lY * 100.0;
        position.z += deltaMove * lZ * 100.0;
    }
    if(deltaStrafe)
    {
        position.x += deltaStrafe * -lZ * 100.0;
        position.z += deltaStrafe * lX * 100.0;
    }
}

void SceneCamera::DebugInfo()
{
    std::cout << "position " << position.x << " " << position.y << " " << position.z << std::endl
              << "angleX " << lX << "angleY " << lY << "angleZ " << lZ << std::endl
              << "upVector " << upVector.x << " " << upVector.y << " " << upVector.z << std::endl;
}
