#include "SceneCamera.h"

SceneCamera::SceneCamera()
{
    position = Vector3(0.0,  0.0, 0.0);
    upVector = Vector3(0.0, 1.0, 0.0);
    angleX = 0;
    angleY = 0;
    lX = sin(angleX * DEG2RAD);
    lZ = -cos(angleX * DEG2RAD);
    lY = -sin(angleY * DEG2RAD);
    matrixToWorld = Matrix4();
}
SceneCamera::~SceneCamera(){}

void SceneCamera::RotateY(float angle)
{
    angleX += angle;
    lX = sin(angleX * DEG2RAD);
    lZ = -cos(angleX * DEG2RAD);

    matrixToWorld = Matrix4().rotateY(angleX).translate(position);
}

void SceneCamera::Avance(float dist)
{
    Vector3 deplacement = Vector3(dist * lX, dist * lY, dist * lZ);
    position.x += deplacement.x;
    position.y += deplacement.y;
    position.z += deplacement.z;

    matrixToWorld = Matrix4().rotateY(angleX).translate(position);
}

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
              << "angleX " << angleX << " angleY " << angleY << std::endl
              << "thetaX " << lX << " thetaY " << lY << " thetaZ " << lZ << std::endl
              << "upVector " << upVector.x << " " << upVector.y << " " << upVector.z << std::endl;
}
