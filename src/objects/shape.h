#pragma once

#include "vec3.h"

#include <vector>

namespace Jam3D {

class Shape
{
public:
    Shape();

    void Move(Jam3D::Vec3<float> translation);
    void SetPosition(Jam3D::Vec3<float> position);
    void Rotate(Jam3D::Vec3<float> rotation);
    void SetRotation(Jam3D::Vec3<float> rotation);
    virtual void Update() = 0;

    Jam3D::Vec3<float> m_Center;
    Jam3D::Vec3<float> m_Rotation;

    std::vector<float> m_VertexData; // 3 position coordinates, 2 texture coordinates, 3 normal vector values 
	std::vector<unsigned int> m_Indices;
};

}