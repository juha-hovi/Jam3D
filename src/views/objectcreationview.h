#pragma once

#include "view.h"
#include "perspectivecamera.h"
#include "orthocamera.h"

namespace Jam3D {

class ObjectCreationView : public View
{
public:
    ObjectCreationView(std::shared_ptr<GLWindow> window);
    void Render() override;

    void InitViewports() override;
    void InitCameras() override;

    void RenderImGui();

    void CursorPosCallback(double xPos, double yPos) override;
	void KeyCallback(int key, int scancode, int action, int mods) override;
	void MouseButtonCallback(int button, int action, int mods) override;
	void ScrollCallback(double yOffset) override;

    void StartDrawing();
    void SaveTempBox();
    void CancelDrawing();

    void UpdateTempBox(OrthoCamera& camera, Viewport& viewport, double xPos, double yPos, bool x, bool y, bool z);

    int m_UpperLeftViewportIndex;
    int m_UpperRightViewportIndex;
    int m_LowerLeftViewportIndex;
    int m_LowerRightViewportIndex;
    int m_ShadowViewportIndex;

    bool m_DrawPlanes = true;

    bool m_MouseRightPressedUpperLeft;

    bool m_MouseLeftPressedUpperLeft;
    bool m_MouseLeftPressedUpperRight;
    bool m_MouseLeftPressedLowerLeft;
    bool m_MouseLeftPressedLowerRight;

    glm::vec3 m_MouseLeftPressLocation;

    std::shared_ptr<PerspectiveCamera> m_UpperLeftCamera;

    std::shared_ptr<OrthoCamera> m_UpperRightCamera;
    std::shared_ptr<OrthoCamera> m_LowerLeftCamera;
    std::shared_ptr<OrthoCamera> m_LowerRightCamera;

    Jam3D::Vec3<float> m_TempBoxCenter;
    Jam3D::Vec3<float> m_TempBoxDimensions;
    Jam3D::Vec3<float> m_TempBoxRotation;
};

}