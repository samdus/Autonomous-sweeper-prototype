#include "Config.h"

Config::Config()
{
}

Config::~Config()
{
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

        std::cout << "var not found" << std::endl;
        return "0";
    }

        std::cout << "file not open" << std::endl;
    return "0";
}
