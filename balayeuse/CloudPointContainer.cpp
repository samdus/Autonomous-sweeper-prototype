#include "CloudPointContainer.h"

CloudPointContainer::CloudPointContainer()
{
    for(size_t i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
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

int CloudPointContainer::ProchainIndex(size_t index)
{
    if(++index >= CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return 0;
    }
    return index;
}

int CloudPointContainer::IndexPrecedent(size_t index)
{
    if(--index > CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return CLOUD_POINT_CIRCULAR_BUFFER - 1;
    }
    return index;
}

bool CloudPointContainer::Insert(std::vector<uint8_t>& rgbBuffer, std::vector<Vector3>& depthBuffer)
{
    Mutex::ScopedLock lock(Array_mutex[indexInsertion]);
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

const std::vector<uint8_t>& CloudPointContainer::GetCloudPointColor(size_t idx)const
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

const std::vector<Vector3>& CloudPointContainer::GetCloudPointDepth(size_t idx)const
{
    if(idx >=0 && idx < CLOUD_POINT_CIRCULAR_BUFFER)
    {
        return depth[idx];
    }
    return depth[indexInsertion];
}

int CloudPointContainer::GetCopyCloudPointToConvert(std::vector<Vector3>& outPoints)
{
    Mutex::ScopedLock lock(Array_mutex[indexSuppression]);
    if(Converted[indexSuppression] == false)
    {
        outPoints = depth[indexSuppression];
        indexSuppression = ProchainIndex(indexSuppression);
        return IndexPrecedent(indexSuppression);
    }

    return -1;
}

/*void CloudPointContainer::EcrirePoint()
{
    std::fstream Fichier;
    std::string line;
    std::string nomFichier = Config::Instance().GetString("FICHIER_DE_POINT");
    Fichier.open(nomFichier);
    std::cout << "Ecriture dans le fichier " << nomFichier << std::endl;
    if(Fichier.is_open())
    {
        for(size_t i = 0; i < depth[0].size(); ++i)
        {
            Fichier << depth[0][i].x << " " << depth[0][i].y << " " << depth[0][i].z << std::endl;
        }
        Fichier.close();
        std::cout << "Fin de l'ecriture dans le fichier " << nomFichier << std::endl;
    }
    else
    {
        std::cout << "Error de l'ecriture dans le fichier " << nomFichier << std::endl;
    }
}*/

void CloudPointContainer::PrintDebugInfo()
{
    std::cout << "===CloudContainer===" << std::endl;
    std::cout << "indexInsertion " << indexInsertion << std::endl;
    std::cout << "indexSuppression " << indexSuppression << std::endl;
    std::cout << "rgb.size() " << CLOUD_POINT_CIRCULAR_BUFFER << std::endl;
    for(size_t i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        std::cout << rgb[i].size() << " ";
    }
    std::cout << "\ndepth.size() " << CLOUD_POINT_CIRCULAR_BUFFER << std::endl;
    for(size_t i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        std::cout << depth[i].size() << " ";
    }
    std::cout << "\nConverted " << CLOUD_POINT_CIRCULAR_BUFFER << std::endl;
    for(size_t i = 0; i < CLOUD_POINT_CIRCULAR_BUFFER; ++i)
    {
        std::cout << Converted[i] << " ";
    }
}
