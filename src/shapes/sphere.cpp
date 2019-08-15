#include "sphere.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Jam3D {

Sphere::Sphere(float radius, Vec3 center, int sectorCount, int stackCount)
	: m_SectorCount(sectorCount), m_StackCount(stackCount), m_Radius(radius), 
	m_Center(center), m_Rotation({90.0f, 0.0f, 0.0f})
{
	Update();
}

Sphere::Sphere(const Sphere& orig)
	: m_SectorCount(orig.m_SectorCount), m_StackCount(orig.m_StackCount), m_Radius(orig.m_Radius),
	m_Center(orig.m_Center), m_Rotation(orig.m_Rotation)
{
	Update();
}

bool Sphere::operator=(const Sphere& rhs)
{
	m_SectorCount = rhs.m_SectorCount;
	m_StackCount = rhs.m_StackCount;
	m_Radius = rhs.m_Radius;
	m_Center = rhs.m_Center;
	m_Rotation = rhs.m_Rotation;
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

void Sphere::Rotate(Vec3 rotation)
{
	m_Rotation = rotation;
}

void Sphere::Update()
{
	m_Positions.clear();
	m_Indices.clear();

	float x, y, z, xy;
	float u, v;

	float sectorStep = 2 * M_PI / m_SectorCount;
	float stackStep = M_PI / m_StackCount;
	float sectorAngle, stackAngle;

	for(int i = 0; i <= m_StackCount; ++i)
	{
		stackAngle = M_PI / 2 - i * stackStep;
		xy = m_Radius * cosf(stackAngle);
		z = m_Radius * sinf(stackAngle);

		for(int j = 0; j <= m_SectorCount; ++j)
		{
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			m_Positions.push_back(x);
			m_Positions.push_back(y);
			m_Positions.push_back(z);

			u = (float)j / m_SectorCount;
			v = (float)i / m_StackCount;
			m_Positions.push_back(u);
			m_Positions.push_back(v);
		}
	}

	int k1, k2;
	for(int i = 0; i < m_StackCount; ++i)
	{
		k1 = i * (m_SectorCount + 1);
		k2 = k1 + m_SectorCount + 1;

		for(int j = 0; j < m_SectorCount; ++j, ++k1, ++k2)
		{
			if(i != 0)
			{
				m_Indices.push_back(k1);
				m_Indices.push_back(k2);
				m_Indices.push_back(k1 + 1);
			}
			if(i != (m_StackCount-1))
			{
				m_Indices.push_back(k1 + 1);
				m_Indices.push_back(k2);
				m_Indices.push_back(k2 + 1);
			}
		}
	}

}

}