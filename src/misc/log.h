#pragma once

#include <string>

#define LOGGING_LEVEL_INFO

namespace Jam3D {

class Log 
{
public:
    enum : unsigned int {
        INFO, WARNING, ERROR
    };

    static const unsigned int LoggingLevel = INFO;

    static void Info(const std::string& msg);
    static void Warning(const std::string& msg);
    static void Error(const std::string& msg);
};

}