#pragma once

#include "vec2.h"
#include "vec4.h"

namespace Jam3D {

class Viewport
{
public:
    Viewport(Vec4<int> corners);

    void Use();

    Vec4<int> m_Corners;
    Vec2<int> m_Dimensions;
};
    
}