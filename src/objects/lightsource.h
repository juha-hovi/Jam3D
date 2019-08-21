#pragma once

#include "vec3.h"

namespace Jam3D {

class LightSource 
{
public:
    LightSource(unsigned int type, Jam3D::Vec3<float> position_or_direction, Jam3D::Vec3<float> color, float intensity);
    LightSource(const LightSource& orig);
    void Move(Jam3D::Vec3<float> translation);
    void SetDirection(Jam3D::Vec3<float> direction);

    enum : unsigned int {DIRECTIONAL_LIGHT, POINT_LIGHT};
    
    unsigned int m_Type;
    
    union {
        Jam3D::Vec3<float> m_Position;
        Jam3D::Vec3<float> m_Direction;
    };

    Jam3D::Vec3<float> m_Color;

    float m_Intensity;
};

}