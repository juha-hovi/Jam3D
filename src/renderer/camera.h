#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "vec2.h"
#include "glwindow.h"

#include <memory>

namespace Jam3D {

class Camera
{
public:
	struct CameraBaseProperties {
		float pitch;
		float yaw;
		glm::vec3 position;
		glm::vec3 target;
		float near;
		float far;
		Jam3D::Vec2<float> windowDim;
		std::shared_ptr<GLWindow> window;
	};

    Camera(CameraBaseProperties properties);
	virtual void Update() = 0;

    std::shared_ptr<GLWindow> m_Window;

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
};

}