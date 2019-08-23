#pragma once

#include <string>

namespace Jam3D {

class Log 
{
public:
    static void Warning(const std::string&);
    static void Error(const std::string&);
};

}