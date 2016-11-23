#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Config
{
public:
    Config();
    ~Config();

    std::string GetString(std::string var);

private:
    std::fstream RobotConfig;
    std::string RobotConfigFile = "Robot.config";
};
