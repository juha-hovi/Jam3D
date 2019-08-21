#include "lightsource.h"

namespace Jam3D {

LightSource::LightSource(unsigned int type, Jam3D::Vec3<float> position_or_direction, Jam3D::Vec3<float> color, float intensity)
    : m_Type(type), m_Position(position_or_direction), m_Color(color), m_Intensity(intensity)
{

}

LightSource::LightSource(const LightSource& orig)
{
    m_Type = orig.m_Type;
    m_Position = orig.m_Position;
    m_Color = orig.m_Color;
    m_Intensity = orig.m_Intensity;
}

void LightSource::Move(Jam3D::Vec3<float> translation)
{
    m_Position += translation;
}

void LightSource::SetDirection(Jam3D::Vec3<float> direction)
{
    m_Direction = direction;
}
    
}