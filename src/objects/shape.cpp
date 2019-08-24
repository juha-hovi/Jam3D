#include "shape.h"

namespace Jam3D {

Shape::Shape()
{

}

void Shape::Move(Jam3D::Vec3<float> translation)
{
    m_Center += translation;
}

void Shape::SetPosition(Jam3D::Vec3<float> position)
{
    m_Center = position;
}

void Shape::Rotate(Jam3D::Vec3<float> rotation)
{
    m_Rotation += rotation;
}

void Shape::SetRotation(Jam3D::Vec3<float> rotation)
{
    m_Rotation = rotation;
}

}