#include "log.h"

#include <iostream>

namespace Jam3D{

void Log::Info(const std::string& msg)
{
#if defined(LOGGING_LEVEL_INFO)
    std::cout << "[INFO]: " << msg << std::endl;
#endif
}

void Log::Warning(const std::string& msg)
{
#if defined(LOGGING_LEVEL_INFO) || defined(LOGGING_LEVEL_WARNING)
    std::cout << "[WARNING]: " << msg << std::endl;
#endif
}

void Log::Error(const std::string& msg)
{
#if defined(LOGGING_LEVEL_INFO) || defined(LOGGING_LEVEL_WARNING) || defined(LOGGING_LEVEL_ERROR)
    std::cout << "[ERROR]: " << msg << std::endl;
#endif
}

}