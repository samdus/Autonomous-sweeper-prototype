#include "Config.h"

Config Config::m_instance=Config();

Config::Config()
{
}

Config::~Config()
{
}

Config& Config::Instance()
{
    return m_instance;
}

std::string Config::GetString(std::string var)
{
    std::string key;
    std::string line;
    std::size_t pos;

    RobotConfig.open(RobotConfigFile);

    if(RobotConfig.is_open())
    {
        while ( getline (RobotConfig, line) )
        {
            pos = line.find(var);
            if(pos != std::string::npos)
            {
                RobotConfig.close();
                return line.substr(pos + var.length()+1);
            }
        }
        RobotConfig.close();

        return "var not found";
    }

    return "file not open";
}
