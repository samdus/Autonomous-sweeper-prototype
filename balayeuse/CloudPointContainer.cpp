#include "CloudPointContainer.h"

CloudPointContainer::CloudPointContainer()
{
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        rgb[i] = std::vector<uint8_t>(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y*3);
        depth[i] = std::vector<Vector3>();
        Converted[i] = true;
    }
}

CloudPointContainer::~CloudPointContainer()
{
    //clear stuff
}

int CloudPointContainer::ProchainIndex(int index)
{
    if(++index >= CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return 0;
    }
    return index;
}

int CloudPointContainer::IndexPrecedent(int index)
{
    if(--index < 0)
    {
        return CLOUD_POINT_CIRCULAR_BUFFER - 1;
    }
    return index;
}

bool CloudPointContainer::Insert(std::vector<uint8_t>& rgbBuffer, std::vector<Vector3>& depthBuffer)
{
    //Mutex::ScopedLock lock(Array_mutex[indexInsertion]);
    //PrintDebugInfo();
    if(InsertDepth(depthBuffer))
    {
        rgb[IndexPrecedent(indexInsertion)].swap(rgbBuffer);
        //PrintDebugInfo();
        return true;
    }
    return false;
}

bool CloudPointContainer::InsertDepth(std::vector<Vector3>& depthBuffer)
{
    if(Converted[indexInsertion] == false)
    {
        //calibrate sampling
        std::cout << "On doit calibrer le sampling" << std::endl;
        return false;
    }
    depth[indexInsertion].swap(depthBuffer);
    Converted[indexInsertion] = false;
    indexInsertion = ProchainIndex(indexInsertion);
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

int CloudPointContainer::GetCopyCloudPointToConvert(std::vector<Vector3>& outPoints)
{
    if(Converted[indexSuppression] == false)
    {
        //depth[indexSuppression].swap(outPoints);
        outPoints = depth[indexSuppression];
        indexSuppression = ProchainIndex(indexSuppression);
        return IndexPrecedent(indexSuppression);
    }

    return -1;
}

void CloudPointContainer::PrintDebugInfo()
{
    std::cout << "===CloudContainer===" << std::endl;
    std::cout << "indexInsertion " << indexInsertion << std::endl;
    std::cout << "indexSuppression " << indexSuppression << std::endl;
    std::cout << "rgb.size() " << CLOUD_POINT_CIRCULAR_BUFFER << std::endl;
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        std::cout << rgb[i].size() << " ";
    }
    std::cout << "\ndepth.size() " << CLOUD_POINT_CIRCULAR_BUFFER << std::endl;
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        std::cout << depth[i].size() << " ";
    }
    std::cout << "\nConverted " << CLOUD_POINT_CIRCULAR_BUFFER << std::endl;
    for(int i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        std::cout << Converted[i] << " ";
    }
}
