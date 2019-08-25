#include "plane.h"

namespace Jam3D {

Plane::Plane(unsigned int type)
{
    m_Center = Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f);
    m_Rotation = Jam3D::Vec3<float>(0.0f, 0.0f, 0.0f);

    const float dist = 1000.0f;
    const int vertexDataSize = 2 * 3 * (3 + 2 + 3);
    const int indicesSize = 2 * 3;

    unsigned int indices[indicesSize] = {
        0, 1, 2,

        3, 4, 5
    };

    if (type == Plane::XZ)
    {
        float vertexData[vertexDataSize] = {
        -dist, 0.0f, -dist,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         dist, 0.0f, -dist, 10.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         dist, 0.0f,  dist, 10.0f, 10.0f, 0.0f, 1.0f, 0.0f,

        -dist, 0.0f, -dist,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         dist, 0.0f,  dist, 10.0f, 10.0f, 0.0f, 1.0f, 0.0f,
        -dist, 0.0f,  dist,  0.0f, 10.0f, 0.0f, 1.0f, 0.0f,
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

    else if (type == Plane::XY)
    {
        float vertexData[vertexDataSize] = {
        -dist, -dist, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f,
         dist, -dist, 0.0f, 10.0f,  0.0f, 0.0f, 0.0f, 1.0f,
         dist,  dist, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 1.0f,

        -dist, -dist, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f,
         dist,  dist, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 1.0f,
        -dist,  dist, 0.0f, 0.0f,  10.0f, 0.0f, 0.0f, 1.0f,
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

    else if (type == Plane::YZ)
    {
        float vertexData[vertexDataSize] = {
        0.0f, -dist, -dist, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.0f,  dist, -dist, 10.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.0f,  dist,  dist, 10.0f, 10.0f, 1.0f, 0.0f, 0.0f,

        0.0f, -dist, -dist, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.0f,  dist,  dist, 10.0f, 10.0f, 1.0f, 0.0f, 0.0f,
        0.0f, -dist,  dist, 0.0f,  10.0f, 1.0f, 0.0f, 0.0f,
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

void Plane::Update()
{

}

}