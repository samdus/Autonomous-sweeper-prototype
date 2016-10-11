#pragma once

#include "CloudPointContainer.h"

CloudPointContainer::CloudPointContainer()
{
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        rgb[i] = std::vector<uint8_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y*3, 0);
        depth[i] = std::vector<uint16_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y, 0);
    }
}

CloudPointContainer::~CloudPointContainer()
{
    //clear stuff
}

void CloudPointContainer::insert(std::vector<uint8_t> rgbBuffer, std::vector<uint16_t> depthBuffer)//we need a copy
{
    if(++index >= CLOUD_POINT_CIRCULAR_BUFFER)
    {
        index = 0;
    }

    rgb[index] = rgbBuffer;
    depth[index] = depthBuffer;
}

void CloudPointContainer::InsertDepth(std::vector<uint16_t>& depthBuffer)
{
    if(++index >= CLOUD_POINT_CIRCULAR_BUFFER)
    {
        index = 0;
    }

    depth[index].swap(depthBuffer);
}

const std::vector<uint8_t> &CloudPointContainer::GetCloudPointColor()const
{
    return rgb[index];
}

const std::vector<uint16_t> &CloudPointContainer::GetCloudPointDepth()const
{
    return depth[index];
}
