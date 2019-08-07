#pragma once
#include "glm/glm.hpp"

#include <vector>

// Simple box 3D object
class Box
{
public:
	struct Vec3 {
		float x, y, z;
		Vec3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; };
	};

	Box(Vec3 corner0, Vec3 corner1);

	void Update(Vec3 corner0, Vec3 corner1);

private:
	Vec3 m_Corner0;
	Vec3 m_Corner1;

public:
	const int m_PositionsSize;
	const int m_IndicesSize;

	Vec3 m_Dimensions;
	Vec3 m_Center;

	std::vector<float> m_Positions;
	std::vector<unsigned int> m_Indices;
};