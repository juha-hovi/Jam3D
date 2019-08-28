#pragma once

#include "view.h"
#include "perspectivecamera.h"
#include "orthocamera.h"

namespace Jam3D {

class ObjectCreationView : public View
{
public:
    enum : unsigned int {
        DRAW, STRECH, MOVE
    };

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
    void DrawTempBox(glm::vec3, bool x, bool y, bool z);
    void StrechTempBox(glm::vec3, bool x, bool y, bool z);
    void MoveTempBox(glm::vec3, bool x, bool y, bool z);

    void UpdateViewports();
    void ResizeViewports(double pos, bool x, bool y);

    void IsInMargin(glm::vec3 worldCoords, bool x, bool y, bool z);

    int m_UpperLeftViewportIndex;
    int m_UpperRightViewportIndex;
    int m_LowerLeftViewportIndex;
    int m_LowerRightViewportIndex;
    int m_ShadowViewportIndex;

    bool m_MouseRightPressedUpperLeft;

    bool m_MouseLeftPressedUpperLeft;
    bool m_MouseLeftPressedUpperRight;
    bool m_MouseLeftPressedLowerLeft;
    bool m_MouseLeftPressedLowerRight;

    bool m_MouseLeftPressedBetweenLeftRight;
    bool m_MouseLeftPressedBetweenUpperLower;

    bool m_MouseLeftPressedTempBoxXMinusMargin;
    bool m_MouseLeftPressedTempBoxXPlusMargin;
    bool m_MouseLeftPressedTempBoxYMinusMargin;
    bool m_MouseLeftPressedTempBoxYPlusMargin;
    bool m_MouseLeftPressedTempBoxZMinusMargin;
    bool m_MouseLeftPressedTempBoxZPlusMargin;

    glm::vec3 m_MouseLeftPressLocation;

    unsigned int m_CurrentTool;

    Jam3D::Vec3<float> m_TempBoxCenterOriginal;
    Jam3D::Vec3<float> m_TempBoxDimensionsOriginal;

    unsigned int m_ViewportDivider;
    Jam3D::Vec2<float> m_ViewportMiddle;
    Jam3D::Vec2<float> m_ViewportmiddleOriginal;

    std::shared_ptr<PerspectiveCamera> m_UpperLeftCamera;

    std::shared_ptr<OrthoCamera> m_UpperRightCamera;
    std::shared_ptr<OrthoCamera> m_LowerLeftCamera;
    std::shared_ptr<OrthoCamera> m_LowerRightCamera;
};

}