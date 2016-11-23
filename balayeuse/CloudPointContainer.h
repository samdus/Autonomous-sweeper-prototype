#pragma once

#include <vector>
#include "../lib/Vectors.h"
#include <iostream>
#include <fstream>
#include "Config.h"
#include "Mutex.h"

#define IR_CAMERA_RESOLUTION_X 640
#define IR_CAMERA_RESOLUTION_Y 480
#define CLOUD_POINT_CIRCULAR_BUFFER 5

class CloudPointContainer
{
    private:
    size_t indexInsertion = 0;
    size_t indexSuppression = 0;

    std::vector<uint8_t> rgb[CLOUD_POINT_CIRCULAR_BUFFER];
    std::vector<Vector3> depth[CLOUD_POINT_CIRCULAR_BUFFER];
    int ProchainIndex(size_t index);
    int IndexPrecedent(size_t index);
    Mutex Array_mutex[CLOUD_POINT_CIRCULAR_BUFFER];

    public:
    bool Converted[CLOUD_POINT_CIRCULAR_BUFFER];

    CloudPointContainer();

    ~CloudPointContainer();

    bool Insert(std::vector<uint8_t>& rgbBuffer, std::vector<Vector3>& depthBuffer);
    bool InsertDepth(std::vector<Vector3>& depthBuffer);

    const std::vector<uint8_t>& GetCloudPointColor()const;
    const std::vector<uint8_t>& GetCloudPointColor(size_t index)const;

    const std::vector<Vector3>& GetCloudPointDepth()const;
    const std::vector<Vector3>& GetCloudPointDepth(size_t idx)const;

    int GetCopyCloudPointToConvert(std::vector<Vector3>& outPoints);

    void EcrirePoint();
    void PrintDebugInfo();
};
