#include "log.h"

#include <iostream>

namespace Jam3D{

void Log::Warning(const std::string& msg)
{
    std::cout << "[Warning]: " << msg << std::endl;
}

void Log::Error(const std::string& msg)
{
    std::cout << "[Error]: " << msg << std::endl;
}

}