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

void CloudPointContainer::Insert(std::vector<uint8_t>& rgbBuffer, std::vector<Vector3>& depthBuffer)
{
    InsertDepth(depthBuffer);
    rgb[index].swap(rgbBuffer);
}

void CloudPointContainer::InsertDepth(std::vector<Vector3>& depthBuffer)
{
    if(++index >= CLOUD_POINT_CIRCULAR_BUFFER)
    {
        index = 0;
    }
    depth[index].swap(depthBuffer);
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
