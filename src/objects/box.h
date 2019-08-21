#pragma once

#include "glm/glm.hpp"

#include "vec3.h"
#include "shape.h"

namespace Jam3D {

// Simple box 3D object
class Box : public Shape
{
public:
	Box(Jam3D::Vec3<float> center, Jam3D::Vec3<float> dimensions, Jam3D::Vec3<float> rotation);
	Box(const Box& orig);
	bool operator=(const Box& rhs);

	void Update() override;

	Jam3D::Vec3<float> m_Dimensions;
};

}