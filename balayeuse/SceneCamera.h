#pragma once

#include <iostream>

class SceneCamera
{
public:
    float posX = 0.0, posY = 0.0, posZ = 0.0,                   //position
          upX = 0.0, upY = -1.0, upZ = 0.0,                      //up vector
          angleX = 0.0, angleY = 0.0,                           //absolute angle
          deltaMove = 0.0, deltaStrafe = 0.0,                   //delta move
          lX = 0.0, lY = 0.0, lZ = 0.0,                         //line of sight
          zoom = 1;                                             //zoom factor

    SceneCamera();
    ~SceneCamera();

    void Update();

    void DebugInfo();
};
