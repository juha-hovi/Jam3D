#pragma once

#include "glm/glm.hpp"

#include "vec3.h"
#include "shape.h"

namespace Jam3D {

// Simple box 3D object
class Box : public Shape
{
public:
	Box(Vec3 corner0, Vec3 corner1);
	Box(const Box& orig);
	bool operator=(const Box& rhs);

	void SetCorners(Vec3 corner0, Vec3 corner1);
	void Update() override;

	Vec3 m_Corner0;
	Vec3 m_Corner1;

	Vec3 m_Dimensions;
};

}