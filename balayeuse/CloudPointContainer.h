#pragma once

#include <vector>
#include <iostream>

#define IR_CAMERA_RESOLUTION_X 640
#define IR_CAMERA_RESOLUTION_Y 480
#define CLOUD_POINT_CIRCULAR_BUFFER 10

class CloudPointContainer
{
    private:
    int index = 0;
    std::vector<uint8_t> rgb[CLOUD_POINT_CIRCULAR_BUFFER];
    std::vector<uint16_t> depth[CLOUD_POINT_CIRCULAR_BUFFER];

    public:
    CloudPointContainer();

    ~CloudPointContainer();

    void insert(std::vector<uint8_t> rgbBuffer, std::vector<uint16_t> depthBuffer);//we need a copy

    const std::vector<uint8_t> &GetCloudPointColor()const;

    const std::vector<uint16_t> &GetCloudPointDepth()const;
};
