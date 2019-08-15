#include "sphere.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Jam3D {

Sphere::Sphere(float radius, Vec3 center, int sectorCount, int stackCount)
	: m_SectorCount(sectorCount), m_StackCount(stackCount), m_Radius(radius), 
	m_Center(center), m_PositionsSize(0), m_IndicesSize(0)
{
	Update();
}

Sphere::Sphere(const Sphere& orig)
	: m_SectorCount(orig.m_SectorCount), m_StackCount(orig.m_StackCount), m_Radius(orig.m_Radius),
	m_Center(orig.m_Center), m_PositionsSize(0), m_IndicesSize(0)
{
	Update();
}

bool Sphere::operator=(const Sphere& rhs)
{
	m_Radius = rhs.m_Radius;
	m_Center = rhs.m_Center;
	Update();
}

void Sphere::Move(Vec3 translation)
{
	m_Center += translation;
	Update();
}

void Sphere::Resize(float radius)
{
	m_Radius = radius;
	Update();
}

void Sphere::Update()
{
	m_Positions.clear();
	m_Indices.clear();

	float x, y, z, u, v;
	float xyPlaneProj;
	float sectorAngleCurrent, stackAngleCurrent;

	float sectorAngleStep = 2 * M_PI / m_SectorCount; 
	float stackAngleStep = M_PI / m_StackCount;

	unsigned int sectorTopIdx, sectorBottomIdx;

	for (int i = 0; i < m_StackCount; ++i)
	{
		stackAngleCurrent = M_PI / 2 - i / stackAngleStep;
		xyPlaneProj = m_Radius * cosf(stackAngleCurrent);
		z = m_Radius * sinf(stackAngleCurrent);
		v = (float)i / m_StackCount;

		sectorTopIdx = i * (m_SectorCount + 1);
		sectorBottomIdx = sectorTopIdx + m_SectorCount + 1;

		for (int j = 0; j < m_SectorCount; ++j)
		{
			sectorAngleCurrent = j * sectorAngleStep;

			x = xyPlaneProj * cosf(sectorAngleCurrent);
			y = xyPlaneProj * sinf(sectorAngleCurrent);
			u = (float)j / m_SectorCount;

			m_Positions.push_back(x);
			m_Positions.push_back(y);
			m_Positions.push_back(z);
			m_Positions.push_back(u);
			m_Positions.push_back(v);

			if (i != 0)
			{
				m_Indices.push_back(sectorTopIdx);
				m_Indices.push_back(sectorBottomIdx);
				m_Indices.push_back(sectorTopIdx + 1);
			}
			if (i != (m_StackCount - 1))
			{
				m_Indices.push_back(sectorTopIdx + 1);
				m_Indices.push_back(sectorBottomIdx);
				m_Indices.push_back(sectorBottomIdx + 1);
			}
		}
	}

	m_PositionsSize = m_Positions.size();
	m_IndicesSize = m_Indices.size();
}

}