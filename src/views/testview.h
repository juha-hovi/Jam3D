#pragma once

#include "view.h"
#include "perspectivecamera.h"

namespace Jam3D {

class TestView : public View
{
public:
    TestView(std::shared_ptr<GLWindow> window);
    void Render() override;

    void CursorPosCallback(double xPos, double yPos) override;
	void KeyCallback(int key, int scancode, int action, int mods) override;
	void MouseButtonCallback(int button, int action, int mods) override;
	void ScrollCallback(double yOffset) override;

private:
    void RenderImGui();
    void InitViewports() override;
    void InitCameras() override;

public:
    std::shared_ptr<PerspectiveCamera> m_Camera;

private:
    Jam3D::Vec3<float> m_BoxCenter;
    Jam3D::Vec3<float> m_BoxDimensions;
    Jam3D::Vec3<float> m_BoxRotation;

    Jam3D::Vec3<float> m_SphereCenter;
    float m_SphereRadius;
    int m_SphereSectorCount;
    int m_SphereStackCount;

    unsigned int m_LightType;
    Jam3D::Vec3<float> m_LightPosition;
    Jam3D::Vec3<float> m_LightColor;
    float m_LightIntensity;

    int m_NormalViewportIndex;
    int m_ShadowViewportIndex;
};

}