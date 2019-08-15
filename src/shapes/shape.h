#pragma once

#include "vec3.h"

#include <vector>

namespace Jam3D {

class Shape
{
public:
    Shape();

    void Move(Vec3 translation);
    void Rotate(Vec3 rotation);
    virtual void Update();

    Vec3 m_Center;
    Vec3 m_Rotation;

    std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
};

}