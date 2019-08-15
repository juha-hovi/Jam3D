#pragma once

#include "vec3.h"

namespace Jam3d {

class Sphere
{
	Sphere(float radius, Vec3 center);
	Sphere(const Sphere& orig);
	bool operator=(const Sphere& rhs);

	void Move(Vec3 translation);
	void Resize(float radius)
	void Update();

	int m_SectorCount;
	int m_StackCount;

	float m_Radius;
	Vec3 m_Center;

	const int m_PositionsSize;
	const int m_IndicesSize;

	std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
}

}