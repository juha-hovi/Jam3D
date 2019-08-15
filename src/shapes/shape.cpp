#include "shape.h"

namespace Jam3D {

Shape::Shape()
    : m_Center({0.0f, 0.0f, 0.0f}), m_Rotation({0.0f, 0.0f, 0.0f})
{

}

void Shape::Move(Vec3 translation)
{
    m_Center += translation;
    Update();
}

void Shape::Rotate(Vec3 rotation)
{
    m_Rotation += rotation;
    Update();
}

void Shape::Update()
{

}

}