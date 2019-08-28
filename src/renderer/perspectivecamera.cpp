#include "perspectivecamera.h"

#include <cmath>

namespace Jam3D {

PerspectiveCamera::PerspectiveCamera(float fov, glm::vec3 target, CameraBaseProperties properties)
	: Camera(properties), m_FoV(fov),
	m_RotationSensitivity(0.07f), m_TranslationSensitivity(15.0f), m_ScrollSensitivity(50.0f),
	m_PressedW(false), m_PressedA(false), m_PressedS(false), m_PressedD(false),
	m_MousePosPrevious(Vec2<float>(0.0f, 0.0f))
{
	m_ProjectionMatrix = glm::mat4(glm::perspective(glm::radians(m_FoV), m_WindowDimension.x / m_WindowDimension.y, m_Near, m_Far));

	double xPos, yPos;
	glfwGetCursorPos(m_Window->m_Window, &xPos, &yPos);
	m_MousePosPrevious.x = (float)xPos;
	m_MousePosPrevious.y = (float)yPos;
	
	Update();
}

void PerspectiveCamera::Update()
{
	ProcessInput();

	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	m_CameraZ = glm::normalize(m_CameraZ);
	m_CameraX = glm::normalize(glm::cross(worldUp, m_CameraZ));
	m_CameraY = glm::cross(m_CameraZ, m_CameraX);

	m_ViewMatrix = glm::lookAt(m_Position, m_Position - m_CameraZ, m_CameraY);
}

void PerspectiveCamera::UpdateProjMat(float aspectRatio)
{
	m_ProjectionMatrix = glm::mat4(glm::perspective(glm::radians(m_FoV), aspectRatio, m_Near, m_Far));
}

void PerspectiveCamera::ProcessInput()
{
	if (m_PressedW)
		m_Position += -m_CameraZ * m_TranslationSensitivity;

	if (m_PressedS)
		m_Position -= -m_CameraZ * m_TranslationSensitivity;

	if (m_PressedA)
		m_Position -= m_CameraX * m_TranslationSensitivity;

	if (m_PressedD)
		m_Position += m_CameraX * m_TranslationSensitivity;

}

void PerspectiveCamera::CursorPosCallback(double xPos, double yPos)
{
	int stateMouseRight = glfwGetMouseButton(m_Window->m_Window, GLFW_MOUSE_BUTTON_RIGHT);

	if (stateMouseRight == GLFW_PRESS)
	{
		float xDiff = ((float)xPos - m_MousePosPrevious.x) * m_RotationSensitivity;
		float yDiff = ((float)yPos - m_MousePosPrevious.y) * m_RotationSensitivity;
		m_Pitch -= yDiff;
		m_Yaw += xDiff;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		m_CameraZ.x = std::cos(glm::radians(m_Pitch)) * std::cos(glm::radians(m_Yaw));
		m_CameraZ.y = std::sin(glm::radians(m_Pitch));
		m_CameraZ.z = std::cos(glm::radians(m_Pitch)) * std::sin(glm::radians(m_Yaw));
	}

	m_MousePosPrevious.x = (float)xPos;
	m_MousePosPrevious.y = (float)yPos;
}

void PerspectiveCamera::KeyCallback(int key, int scancode, int action, int mods)
{
	bool flag = false;
	if (action == GLFW_PRESS)
		flag = true;
	else if (action == GLFW_REPEAT)
		flag = true;
	else if (action == GLFW_RELEASE)
		flag = false;

	switch(key){
		case GLFW_KEY_W: 
			m_PressedW = flag;
			break;

		case GLFW_KEY_S: 
			m_PressedS = flag;
			break;
		
		case GLFW_KEY_A:
			m_PressedA = flag;
			break;

		case GLFW_KEY_D: 
			m_PressedD = flag;
			break;
	}
}

void PerspectiveCamera::MouseButtonCallback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		glfwSetInputMode(m_Window->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	else
		glfwSetInputMode(m_Window->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void PerspectiveCamera::ScrollCallback(double yOffset)
{
	m_Position += -m_CameraZ * m_ScrollSensitivity * (float)yOffset;
}

}