#pragma once

#include "vec3.h"
#include "shape.h"

namespace Jam3D {

class Sphere : public Shape
{
public:
	Sphere(float radius, Vec3 center, int sectorCount, int stackCount);
	Sphere(const Sphere& orig);
	bool operator=(const Sphere& rhs);

	void Resize(float radius);
	void Update() override;

	int m_SectorCount;
	int m_StackCount;

	float m_Radius;
};

}