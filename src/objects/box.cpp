#include "box.h"
#include <cmath>

namespace Jam3D {

// Builds a 3D box. See tex_example_full.png for texture layout.
// m_Positions.size() = 120: 24 vertices, 3 coordinates, 2 texture coordinates
// m_Indices.size() = 36: 6 sides, 2 triangles per side, 3 vertices per triangle
Box::Box(Jam3D::Vec3<float> center, Jam3D::Vec3<float> dimensions, Jam3D::Vec3<float> rotation)
{
	m_Center = center;
	m_Dimensions = dimensions;
	m_Rotation = rotation;
	Update();
}

Box::Box(const Box& orig)
{
	m_Rotation = orig.m_Rotation;
	m_Center = orig.m_Center;
	m_Dimensions = orig.m_Dimensions;
	Update();
}

bool Box::operator=(const Box& rhs)
{	
	m_Rotation = rhs.m_Rotation;
	m_Center = rhs.m_Center;
	m_Dimensions = rhs.m_Dimensions;
	Update();
}

// Update the box.
// Input: new corners
void Box::Update()
{
	/*
			7-----6
		3---+-2	  |
		|	4-+---5
		0-----1
	*/
	Jam3D::Vec3<float> corner0 = m_Dimensions / -2.0f;
	Jam3D::Vec3<float> corner1 = m_Dimensions / 2.0f;

	const int vertexDataSize = 6 * 4 * (3 + 2 + 3);
    const int indicesSize = 6 * 2 * 3;

	float vertexData[vertexDataSize] = {
		// Front
		corner0.x, corner0.y, corner1.z, 2.0f / 3.0f, 0.0f,	0.0f, 0.0f, 1.0f, 	// 0
		corner1.x, corner0.y, corner1.z, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 	// 1
		corner1.x, corner1.y, corner1.z, 1.0f, 0.5f,		0.0f, 0.0f, 1.0f, 	// 2
		corner0.x, corner1.y, corner1.z, 2.0f / 3.0f, 0.5f,	0.0f, 0.0f, 1.0f, 	// 3

		// Back
		corner1.x, corner0.y, corner0.z, 1.0f / 3.0f, 0.0f,	0.0f, 0.0f, -1.0f,	// 5
		corner0.x, corner0.y, corner0.z, 2.0f / 3.0f, 0.0f,	0.0f, 0.0f, -1.0f,	// 4
		corner0.x, corner1.y, corner0.z, 2.0f / 3.0f, 0.5f,	0.0f, 0.0f, -1.0f,	// 7
		corner1.x, corner1.y, corner0.z, 1.0f / 3.0f, 0.5f,	0.0f, 0.0f, -1.0f,	// 6

		// Left
		corner0.x, corner0.y, corner0.z, 0.0f, 0.5f,		-1.0f, 0.0f, 0.0f,	// 4
		corner0.x, corner0.y, corner1.z, 1.0f / 3.0f, 0.5f,	-1.0f, 0.0f, 0.0f,	// 0
		corner0.x, corner1.y, corner1.z, 1.0f / 3.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	// 3
		corner0.x, corner1.y, corner0.z, 0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	// 7

		// Right
		corner1.x, corner0.y, corner1.z, 1.0f / 3.0f, 0.5f,	1.0f, 0.0f, 0.0f,	// 1
		corner1.x, corner0.y, corner0.z, 2.0f / 3.0f, 0.5f,	1.0f, 0.0f, 0.0f,	// 5
		corner1.x, corner1.y, corner0.z, 2.0f / 3.0f, 1.0f,	1.0f, 0.0f, 0.0f,	// 6
		corner1.x, corner1.y, corner1.z, 1.0f / 3.0f, 1.0f,	1.0f, 0.0f, 0.0f,	// 2

		// Top
		corner0.x, corner1.y, corner1.z, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	// 3
		corner1.x, corner1.y, corner1.z, 1.0f / 3.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// 2
		corner1.x, corner1.y, corner0.z, 1.0f / 3.0f, 0.5f,	0.0f, 1.0f, 0.0f,	// 6
		corner0.x, corner1.y, corner0.z, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f,	// 7

		// Bottom
		corner1.x, corner0.y, corner1.z, 1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	// 1
		corner0.x, corner0.y, corner1.z, 2.0f / 3.0f, 1.0f,	0.0f, -1.0f, 0.0f,	// 0
		corner0.x, corner0.y, corner0.z, 2.0f / 3.0f, 0.5f,	0.0f, -1.0f, 0.0f,	// 4
		corner1.x, corner0.y, corner0.z, 1.0f, 0.5f,		0.0f, -1.0f, 0.0f	// 5
	};

	unsigned int indices[indicesSize] = {
		0, 1, 2, // Front
		0, 2, 3,

		4, 5, 6, // Back
		4, 6, 7,

		8, 9, 10, // Left
		8, 10, 11,

		12, 13, 14, // Right
		12, 14, 15,

		16, 17, 18, // Top
		16, 18, 19,

		20, 21, 22, // Bottom
		20, 22, 23
	};

	m_VertexData.clear();
	for (int i = 0; i < vertexDataSize; ++i)
	{
		m_VertexData.push_back(vertexData[i]);
	}
	m_Indices.clear();
	for (int i = 0; i < indicesSize; ++i)
	{
		m_Indices.push_back(indices[i]);
	}
}

}