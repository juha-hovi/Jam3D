#include "camera.h"

#include <iostream>

namespace Jam3D {

Camera::Camera(float fov, float near, float far, Jam3D::Vec2 windowDim, GLFWwindow* window)
	: m_Window(window), m_FoV(fov), m_Near(near), m_Far(far), m_WindowDimension(windowDim),
	m_FocusPoint({0.0f, 0.0f}), m_FocusPointOld({0.0f, 0.0f}), m_FocusPointDistance(1000.0f), m_Rotation({0.0f, 0.0f, 0.0f}),
	m_RotationOld({0.0f, 0.0f, 0.0f}), m_RotationSensitivity(0.5), m_TranslationSensitivity(1),
	m_ScrollSensitivity(100), m_RotationCursorOriginX(0.0), m_RotationCursorOriginY(0.0),
	m_TranslationCursorOriginX(0.0), m_TranslationCursorOriginY(0.0)
{
	m_ProjectionMatrix = glm::mat4(glm::perspective(glm::radians(m_FoV), m_WindowDimension.x / m_WindowDimension.y, m_Near, m_Far));
	m_ViewMatrix = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_FocusPointDistance)));
}

void Camera::Update()
{
	m_ViewMatrix = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_FocusPointDistance)));

	m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(m_FocusPoint.x, m_FocusPoint.y, 0.0f));

	m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::CursorPosCallback(double xPos, double yPos)
{
	// Rotation
	int stateLeft = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT);
	if (stateLeft == GLFW_PRESS)
	{
	    m_Rotation.y = m_RotationOld.y + m_RotationSensitivity * (xPos - m_RotationCursorOriginX);
		m_Rotation.x = m_RotationOld.x + m_RotationSensitivity * (yPos - m_RotationCursorOriginY);
	}
	else
	{
		m_RotationCursorOriginX = xPos;
		m_RotationCursorOriginY = yPos;
		m_RotationOld.x = m_Rotation.x;
		m_RotationOld.y = m_Rotation.y;
	}
	// Prevent flipping upside down
	if (m_Rotation.x > 90)
		m_Rotation.x = 90;
	if (m_Rotation.x < -90)
		m_Rotation.x = -90;

	// Translation
	int stateMiddle = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE);
	if (stateMiddle == GLFW_PRESS)
	{
		m_FocusPoint.x = m_FocusPointOld.x + m_TranslationSensitivity * (xPos - m_TranslationCursorOriginX);
		m_FocusPoint.y = m_FocusPointOld.y + m_TranslationSensitivity * -(yPos - m_TranslationCursorOriginY);
	}
	else
	{
		m_TranslationCursorOriginX = xPos;
		m_TranslationCursorOriginY = yPos;
		m_FocusPointOld.x = m_FocusPoint.x;
		m_FocusPointOld.y = m_FocusPoint.y;
	}

	Update();	
}

void Camera::ScrollCallback(double yOffset)
{
	m_FocusPointDistance -= yOffset * m_ScrollSensitivity;
	if (m_FocusPointDistance < 0)
		m_FocusPointDistance = 0;
	
	Update();
}

void Camera::MouseButtonCallback(int button, int action, int mods)
{
	int stateLeft = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
	int stateMiddle = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE);
	if (stateLeft == GLFW_PRESS || stateMiddle == GLFW_PRESS)
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

}