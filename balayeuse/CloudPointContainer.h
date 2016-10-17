#pragma once

#include <vector>
#include <iostream>
#include "Vectors.h"

#define IR_CAMERA_RESOLUTION_X 640
#define IR_CAMERA_RESOLUTION_Y 480
#define CLOUD_POINT_CIRCULAR_BUFFER 10

class CloudPointContainer
{
    private:
    int index = 0;
    std::vector<uint8_t> rgb[CLOUD_POINT_CIRCULAR_BUFFER];
    std::vector<Vector3> depth[CLOUD_POINT_CIRCULAR_BUFFER];

    public:
    CloudPointContainer();

    ~CloudPointContainer();

    void Insert(std::vector<uint8_t>& rgbBuffer, std::vector<uint16_t>& depthBuffer);
    void InsertDepth(std::vector<uint16_t>& depthBuffer);

    const std::vector<uint8_t>& GetCloudPointColor()const;
    const std::vector<uint8_t>& GetCloudPointColor(int index)const;

    const std::vector<Vector3>& GetCloudPointDepth()const;
    const std::vector<Vector3>& GetCloudPointDepth(int idx)const;
};
