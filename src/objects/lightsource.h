#pragma once

#include "vec3.h"

namespace Jam3D {

class LightSource 
{
public:
    LightSource(unsigned int type, Vec3 position_or_direction, Vec3 color, float intensity);
    LightSource(const LightSource& orig);
    void Move(Vec3 translation);
    void SetDirection(Vec3 direction);

    enum : unsigned int {DIRECTIONAL_LIGHT, POINT_LIGHT};
    
    unsigned int m_Type;
    
    union {
        Vec3 m_Position;
        Vec3 m_Direction;
    };

    Vec3 m_Color;

    float m_Intensity;
};

}