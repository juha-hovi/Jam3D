#pragma once

#include "vec3.h"
#include "camera.h"

namespace Jam3D {

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fov, float near, float far, Jam3D::Vec2<float> windowDim, std::shared_ptr<GLWindow> window);
	void Update();
	
	void ProcessInput();

	void CursorPosCallback(double xPos, double yPos);
	void KeyCallback(int key, int scancode, int action, int mods);
	void MouseButtonCallback(int button, int action, int mods);
	void ScrollCallback(double yOffset);

	float m_FoV;

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