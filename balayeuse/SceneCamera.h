#pragma once

#include <iostream>
#include "../lib/Vectors.h"
#include "Matrices.h"

class SceneCamera
{

public:
    Vector3 position;
    Vector3 upVector;
    Matrix4 matrixToWorld;

    float angleX = 0.0, angleY = 0.0,                           //absolute angle
          deltaMove = 0.0, deltaStrafe = 0.0,                   //delta move
          lX = 0.0, lY = 0.0, lZ = 0.0,                         //line of sight
          zoom = 1;                                             //zoom factor

    SceneCamera();
    ~SceneCamera();

    void RotateY(float angle);
    void Avance(float dist);

    void Update();

    void DebugInfo();
};
