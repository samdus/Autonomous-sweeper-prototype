#include "CloudPointContainer.h"

CloudPointContainer::CloudPointContainer()
{
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        rgb[i] = std::vector<uint8_t>();
        depth[i] = std::vector<Vector3>();
        Converted[i] = true;
    }
}

CloudPointContainer::~CloudPointContainer()
{
    //clear stuff
}

bool CloudPointContainer::Insert(std::vector<uint8_t>& rgbBuffer, std::vector<Vector3>& depthBuffer)
{
    if(InsertDepth(depthBuffer))
    {
        rgb[indexInsertion].swap(rgbBuffer);
        return true;
    }
    return false;
}

bool CloudPointContainer::InsertDepth(std::vector<Vector3>& depthBuffer)
{
    if(++indexInsertion >= CLOUD_POINT_CIRCULAR_BUFFER)
    {
        indexInsertion = 0;
    }
    if(Converted[indexInsertion] == false)
    {
        --indexInsertion;
        return false;
    }
    depth[indexInsertion].swap(depthBuffer);
    Converted[indexInsertion] = false;
    return true;
}

const std::vector<uint8_t>& CloudPointContainer::GetCloudPointColor()const
{
    return rgb[indexInsertion];
}

const std::vector<uint8_t>& CloudPointContainer::GetCloudPointColor(int idx)const
{
    if(idx >=0 && idx < CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return rgb[idx];
    }
    return rgb[indexInsertion];
}

const std::vector<Vector3>& CloudPointContainer::GetCloudPointDepth()const
{
    return depth[indexInsertion];
}

const std::vector<Vector3>& CloudPointContainer::GetCloudPointDepth(int idx)const
{
    if(idx >=0 && idx < CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return depth[idx];
    }
    return depth[indexInsertion];
}

bool CloudPointContainer::GetCloudPointToConvert(std::vector<Vector3>& outPoints)
{
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        if(Converted[i] == false)
        {
            depth[i].swap(outPoints);
            return true;
        }
    }
    return false;
}
