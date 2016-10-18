#include "CloudPointContainer.h"

CloudPointContainer::CloudPointContainer()
{
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        rgb[i] = std::vector<uint8_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y*3);
        depth[i] = std::vector<Vector3>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y);
    }
}

CloudPointContainer::~CloudPointContainer()
{
    //clear stuff
}

void CloudPointContainer::Insert(std::vector<uint8_t>& rgbBuffer, std::vector<uint16_t>& depthBuffer)
{
    InsertDepth(depthBuffer);
    rgb[index].swap(rgbBuffer);
}

void CloudPointContainer::InsertDepth(std::vector<uint16_t>& depthBuffer)
{
    if(++index >= CLOUD_POINT_CIRCULAR_BUFFER)
    {
        index = 0;
    }

    //transforme les donnees du buffer en coordonne monde
    float f = 595.f;
    for(int i = 0; i < IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y; ++i)
    {
        depth[index][i].x = (i%IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_X-1)/2.f) * depthBuffer[i] / f;
        depth[index][i].y = (i/IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_Y-1)/2.f) * depthBuffer[i] / f;
        depth[index][i].z = depthBuffer[i];
    }
}

const std::vector<uint8_t>& CloudPointContainer::GetCloudPointColor()const
{
    return rgb[index];
}

const std::vector<uint8_t>& CloudPointContainer::GetCloudPointColor(int idx)const
{
    if(idx >=0 && idx < CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return rgb[idx];
    }
    return rgb[index];
}

const std::vector<Vector3>& CloudPointContainer::GetCloudPointDepth()const
{
    return depth[index];
}

const std::vector<Vector3>& CloudPointContainer::GetCloudPointDepth(int idx)const
{
    if(idx >=0 && idx < CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return depth[idx];
    }
    return depth[index];
}
