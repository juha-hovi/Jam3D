#include "box.h"

// Builds a 3D box from two given corners.
// m_PositionsSize = 40: 8 vertices, 3 coordinates, 2 texture coordinates
// m_IndicesSize = 36: 6 sides, 2 triangles per side, 3 vertices per triangle
// TODO: 8 vertices is not enough to get texture coordinates correct for all sides?
Box::Box(Vec3 corner0, Vec3 corner1)
	: m_Corner0(corner0), 
	m_Corner1(corner1),
	m_PositionsSize(8 * (3 + 2)),
	m_IndicesSize(6 * 2 * 3)
{
	/*
			7-----6
		3---+-2	  |
		|	4-+---5
		0-----1
	*/

	float positions[m_PositionsSize] = {
		m_Corner0.x, m_Corner0.y, m_Corner0.z, 0.0f, 0.0f,	// 0
		m_Corner1.x, m_Corner0.y, m_Corner0.z, 1.0f, 0.0f,	// 1
		m_Corner1.x, m_Corner1.y, m_Corner0.z, 1.0f, 1.0f,	// 2
		m_Corner0.x, m_Corner1.y, m_Corner0.z, 0.0f, 1.0f,	// 3

		m_Corner0.x, m_Corner0.y, m_Corner1.z, 1.0f, 0.0f,	// 4
		m_Corner1.x, m_Corner0.y, m_Corner1.z, 0.0f, 0.0f,	// 5
		m_Corner1.x, m_Corner1.y, m_Corner1.z, 0.0f, 1.0f,	// 6
		m_Corner0.x, m_Corner1.y, m_Corner1.z, 1.0f, 1.0f	// 7
	};

	unsigned int indices[m_IndicesSize] = {
		0, 1, 3, // Front
		1, 2, 3,

		4, 5, 7, // Back
		5, 6, 7,

		2, 3, 7, // Top
		2, 6, 7,

		0, 3, 7, // Left
		0, 4, 7,

		0, 1, 4, // Bottom
		1, 4, 5,

		1, 2, 5, // Right
		2, 5, 6
	};


	for (int i = 0; i < m_PositionsSize; ++i)
	{
		m_Positions[i] = positions[i];
	}
	for (int i = 0; i < m_IndicesSize; ++i)
	{
		m_Indices[i] = indices[i];
	}
}