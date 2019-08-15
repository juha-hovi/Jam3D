#pragma once

#include "vec3.h"

#include <vector>

namespace Jam3D {

class Sphere
{
public:
	Sphere(float radius, Vec3 center, int sectorCount, int stackCount);
	Sphere(const Sphere& orig);
	bool operator=(const Sphere& rhs);

	void Move(Vec3 translation);
	void Resize(float radius);
	void Update();

	int m_SectorCount;
	int m_StackCount;

	float m_Radius;
	Vec3 m_Center;

	int m_PositionsSize;
	int m_IndicesSize;

	std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
};

}