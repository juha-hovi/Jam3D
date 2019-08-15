#pragma once

#include "glm/glm.hpp"

#include <vector>

#include "vec3.h"

namespace Jam3D {

// Simple box 3D object
class Box
{
public:
	Box(Vec3 corner0, Vec3 corner1);
	Box(const Box& orig);
	bool operator=(const Box& rhs);

	void Move(Vec3 translation);
	void Rotate(Vec3 rotation);
	void SetCorners(Vec3 corner0, Vec3 corner1);
	void Update();

	Vec3 m_Corner0;
	Vec3 m_Corner1;
	Vec3 m_Rotation;

	Vec3 m_Dimensions;
	Vec3 m_Center;

	std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
};

}