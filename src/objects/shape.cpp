#include "shape.h"

namespace Jam3D {

Shape::Shape()
{

}

void Shape::Move(Jam3D::Vec3<float> translation)
{
    m_Center += translation;
    Update();
}

void Shape::Rotate(Jam3D::Vec3<float> rotation)
{
    m_Rotation += rotation;
    Update();
}

void Shape::Update()
{

}

}