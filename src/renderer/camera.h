#pragma once

#include "vec2.h"
#include "vec3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Jam3D {

class Camera
{
public:
	Camera(float fov, float near, float far, Vec2 windowDim);
	void Move(Vec3 moveVec);
	void Zoom(float zoom);
	void Update();

	float m_FoV;
	float m_Near;
	float m_Far;
	Vec2 m_WindowDimension;

	Vec3 m_FocusPoint;
	float m_FocusPointDistance;
	
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
};

}