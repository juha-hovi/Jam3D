#pragma once

#include "view.h"

namespace Jam3D {

class ObjectCreationView : public View
{
public:
    ObjectCreationView(std::shared_ptr<GLWindow> window);
    void Render() override;

    void InitViewports();

    void CursorPosCallback(double xPos, double yPos) override;
	void KeyCallback(int key, int scancode, int action, int mods) override;
	void MouseButtonCallback(int button, int action, int mods) override;
	void ScrollCallback(double yOffset) override;

    int m_UpperLeftViewportIndex;
    int m_UpperRightViewportIndex;
    int m_LowerLeftViewportIndex;
    int m_LowerRightViewportIndex;
};

}