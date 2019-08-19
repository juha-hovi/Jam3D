#include "axes.h"

namespace Jam3D {

Axes::Axes()
{
    m_Center = Vec3(0.0f, 0.0f, 0.0f);
    m_Rotation = Vec3(0.0f, 0.0f, 0.0f);

    const float dist = 2000.0f;
    const int vertexDataSize = 3 * 2 * (3 + 2 + 3);
    const int indicesSize = 3 * 2;

    float vertexData[vertexDataSize] = {
        -dist, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         dist, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, -dist, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f,  dist, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, -dist, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,  dist, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f 
    };

    unsigned int indices[indicesSize] = {
        0, 1,

        2, 3,

        4, 5
    };

	for (int i = 0; i < vertexDataSize; ++i)
	{
		m_VertexData.push_back(vertexData[i]);
	}
	for (int i = 0; i < indicesSize; ++i)
	{
		m_Indices.push_back(indices[i]);
	}
}

}