#pragma once

#include <vector>

namespace Jam3D {

class Axes
{
public:
    Axes();

    std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
};

}