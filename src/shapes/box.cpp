#include "box.h"
#include <cmath>

namespace Jam3D {

// Builds a 3D box from two given corners. See tex_example_full.png for texture layout.
// m_PositionsSize = 40: 24 vertices, 3 coordinates, 2 texture coordinates
// m_IndicesSize = 36: 6 sides, 2 triangles per side, 3 vertices per triangle
Box::Box(Vec3 corner0, Vec3 corner1)
	: m_Corner0(corner0), 
	m_Corner1(corner1),
	m_PositionsSize(24 * (3 + 2)),
	m_IndicesSize(6 * 2 * 3),
	m_Dimensions({0.0f, 0.0f, 0.0f}),
	m_Center({0.0f, 0.0f, 0.0f})
{
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
	m_Corner0 = m_Corner0;
	m_Corner1 = m_Corner1;

	m_Dimensions.x = std::abs(m_Corner1.x - m_Corner0.x);
	m_Dimensions.y = std::abs(m_Corner1.y - m_Corner0.y);
	m_Dimensions.z = std::abs(m_Corner1.z - m_Corner0.z);

	m_Center.x = (m_Corner0.x + m_Corner1.x) / 2.0f;
	m_Center.y = (m_Corner0.y + m_Corner1.y) / 2.0f;
	m_Center.z = (m_Corner0.z + m_Corner1.z) / 2.0f;

	float positions[m_PositionsSize] = {
		// Front
		m_Corner0.x, m_Corner0.y, m_Corner1.z, 2.0f / 3.0f, 0.0f,	// 0
		m_Corner1.x, m_Corner0.y, m_Corner1.z, 1.0f, 0.0f,			// 1
		m_Corner1.x, m_Corner1.y, m_Corner1.z, 1.0f, 0.5f,			// 2
		m_Corner0.x, m_Corner1.y, m_Corner1.z, 2.0f / 3.0f, 0.5f,	// 3

		// Back
		m_Corner1.x, m_Corner0.y, m_Corner0.z, 1.0f / 3.0f, 0.0f,	// 5
		m_Corner0.x, m_Corner0.y, m_Corner0.z, 2.0f / 3.0f, 0.0f,	// 4
		m_Corner0.x, m_Corner1.y, m_Corner0.z, 2.0f / 3.0f, 0.5f,	// 7
		m_Corner1.x, m_Corner1.y, m_Corner0.z, 1.0f / 3.0f, 0.5f,	// 6

		// Left
		m_Corner0.x, m_Corner0.y, m_Corner0.z, 0.0f, 0.5f,			// 4
		m_Corner0.x, m_Corner0.y, m_Corner1.z, 1.0f / 3.0f, 0.5f,	// 0
		m_Corner0.x, m_Corner1.y, m_Corner1.z, 1.0f / 3.0f, 1.0f,	// 3
		m_Corner0.x, m_Corner1.y, m_Corner0.z, 0.0f, 1.0f,			// 7

		// Right
		m_Corner1.x, m_Corner0.y, m_Corner1.z, 1.0f / 3.0f, 0.5f,	// 1
		m_Corner1.x, m_Corner0.y, m_Corner0.z, 2.0f / 3.0f, 0.5f,	// 5
		m_Corner1.x, m_Corner1.y, m_Corner0.z, 2.0f / 3.0f, 1.0f,	// 6
		m_Corner1.x, m_Corner1.y, m_Corner1.z, 1.0f / 3.0f, 1.0f,	// 2

		// Top
		m_Corner0.x, m_Corner1.y, m_Corner1.z, 0.0f, 0.0f,			// 3
		m_Corner1.x, m_Corner1.y, m_Corner1.z, 1.0f / 3.0f, 0.0f,	// 2
		m_Corner1.x, m_Corner1.y, m_Corner0.z, 1.0f / 3.0f, 0.5f,	// 6
		m_Corner0.x, m_Corner1.y, m_Corner0.z, 0.0f, 0.5f,			// 7

		// Bottom
		m_Corner1.x, m_Corner0.y, m_Corner1.z, 1.0f, 1.0f,			// 1
		m_Corner0.x, m_Corner0.y, m_Corner1.z, 2.0f / 3.0f, 1.0f,	// 0
		m_Corner0.x, m_Corner0.y, m_Corner0.z, 2.0f / 3.0f, 0.5f,	// 4
		m_Corner1.x, m_Corner0.y, m_Corner0.z, 1.0f, 0.5f			// 5
	};

	unsigned int indices[m_IndicesSize] = {
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

	m_Positions.clear();
	for (int i = 0; i < m_PositionsSize; ++i)
	{
		m_Positions.push_back(positions[i]);
	}
	m_Indices.clear();
	for (int i = 0; i < m_IndicesSize; ++i)
	{
		m_Indices.push_back(indices[i]);
	}
}

}