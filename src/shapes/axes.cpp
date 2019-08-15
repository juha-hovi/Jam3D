#include "axes.h"

namespace Jam3D {

Axes::Axes()
{
    const float dist = 1000.0f;
    const int positionsSize = 3 * 2 * 5;
    const int indicesSize = 3 * 2;

    float positions[positionsSize] = {
        -dist, 0.0f, 0.0f, 0.1f, 0.1f,
         dist, 0.0f, 0.0f, 0.1f, 0.1f,

        0.0f, -dist, 0.0f, 0.1f, 0.1f,
        0.0f,  dist, 0.0f, 0.1f, 0.1f,

        0.0f, 0.0f, -dist, 0.1f, 0.1f,
        0.0f, 0.0f,  dist, 0.1f, 0.1f,
    };

    unsigned int indices[indicesSize] = {
        // x-axis
        0, 1,
        // y-axis
        2, 3,
        // z-axis
        4, 5
    };

	for (int i = 0; i < positionsSize; ++i)
	{
		m_Positions.push_back(positions[i]);
	}
	for (int i = 0; i < indicesSize; ++i)
	{
		m_Indices.push_back(indices[i]);
	}
}

}