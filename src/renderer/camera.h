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
	Camera(float fov, float near, float far, Vec2<float> windowDim, GLFWwindow* window);
	void Update();
	void ProcessInput();
	void CursorPosCallback(double xPos, double yPos);
	void KeyCallback(int key, int scancode, int action, int mods);
	void MouseButtonCallback(int button, int action, int mods);
	void ScrollCallback(double yOffset);

	GLFWwindow* m_Window;

	float m_FoV;
	float m_Near;
	float m_Far;
	Vec2<float> m_WindowDimension;

	glm::vec3 m_Position;
	float m_Pitch;
	float m_Yaw;

	glm::vec3 m_CameraX;
	glm::vec3 m_CameraY;
	glm::vec3 m_CameraZ;
	
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	float m_RotationSensitivity;
	float m_TranslationSensitivity;
	float m_ScrollSensitivity;

	bool m_PressedW;
	bool m_PressedA;
	bool m_PressedS;
	bool m_PressedD;

	Vec2<float> m_MousePosPrevious;
};

}