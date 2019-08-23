#pragma once

#include <vector>

#include "viewport.h"

namespace Jam3D {

class View
{
public:
    View();

    std::vector<Viewport> m_Viewports;
};

}