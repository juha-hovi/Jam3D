#pragma once

#include "vec3.h"

#include <vector>

namespace Jam3D {

class Shape
{
public:
    void Move(Vec3 translation);
    void Rotate(Vec3 rotation);

    std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
};

}