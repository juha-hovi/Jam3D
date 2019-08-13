#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "vec2.h"
#include "vec3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Jam3D {

class Camera
{
public:
	Camera(float fov, float near, float far, Vec2 windowDim, GLFWwindow* window);
	void Move(Vec3 moveVec);
	void Zoom(float zoom);
	void Update();
	void HandleMouse();

	GLFWwindow* m_Window;

	float m_FoV;
	float m_Near;
	float m_Far;
	Vec2 m_WindowDimension;

	Vec2 m_FocusPoint;
	float m_FocusPointDistance;
	
	glm::vec3 m_Rotation;
	glm::vec3 m_RotationOld;
	
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	double m_CursorPosX;
	double m_CursorPosY;

	double m_RotationSensitivity;

	double m_CursorOriginX;
	double m_CursorOriginY;
};

}