#include "axes.h"

namespace Jam3D {

Axes::Axes()
    : m_PositionsSize(3 * 2 * 5), m_IndicesSize(3 * 2)
{
    // Visualization distance from origin
    float dist = 1000.0f;

    float positions[m_PositionsSize] = {
        -dist, 0.0f, 0.0f, 0.1f, 0.1f,
         dist, 0.0f, 0.0f, 0.1f, 0.1f,

        0.0f, -dist, 0.0f, 0.1f, 0.1f,
        0.0f,  dist, 0.0f, 0.1f, 0.1f,

        0.0f, 0.0f, -dist, 0.1f, 0.1f,
        0.0f, 0.0f,  dist, 0.1f, 0.1f,
    };

    unsigned int indices[m_IndicesSize] = {
        // x-axis
        0, 1,
        // y-axis
        2, 3,
        // z-axis
        4, 5
    };

	for (int i = 0; i < m_PositionsSize; ++i)
	{
		m_Positions.push_back(positions[i]);
	}
	for (int i = 0; i < m_IndicesSize; ++i)
	{
		m_Indices.push_back(indices[i]);
	}
}

}