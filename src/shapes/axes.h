#pragma once

#include <vector>

namespace Jam3D {

class Axes
{
public:
    Axes();

    const int m_PositionsSize;
	const int m_IndicesSize;

    std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
};

}