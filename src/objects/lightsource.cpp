#include "lightsource.h"

namespace Jam3D {

LightSource::LightSource(unsigned int type, Vec3 position_or_direction, Vec3 color, float intensity)
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

void LightSource::Move(Vec3 translation)
{
    m_Position += translation;
}

void LightSource::SetDirection(Vec3 direction)
{
    m_Direction = direction;
}
    
}