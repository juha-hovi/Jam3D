#pragma once

#include "glm/glm.hpp"

#include "vec3.h"
#include "shape.h"

namespace Jam3D {

// Simple box 3D object
class Box : public Shape
{
public:
	Box(Vec3 center, Vec3 dimensions, Vec3 rotation);
	Box(const Box& orig);
	bool operator=(const Box& rhs);

	void Update() override;

	Vec3 m_Dimensions;
};

}