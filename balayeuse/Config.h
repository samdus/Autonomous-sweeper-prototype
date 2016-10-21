#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Config
{
public:
    static Config& Instance();
    std::string GetString(std::string var);

private:
    Config& operator= (const Config&){}
    Config (const Config&){}

    static Config m_instance;
    std::fstream RobotConfig;
    std::string RobotConfigFile = "Robot.config";

    Config();
    ~Config();

};
