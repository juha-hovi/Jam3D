#pragma once

#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "viewport.h"
#include "glwindow.h"
#include "renderer.h"

namespace Jam3D {

class View
{
public:
    View(std::shared_ptr<GLWindow> window);

    virtual void Render() = 0;    

    std::shared_ptr<GLWindow> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    std::vector<Viewport> m_Viewports;

    // Input handling
    static View *m_CurrentView;
    virtual void SetAsCurrent() { m_CurrentView = this; };

    virtual void CursorPosCallback(double xPos, double yPos) = 0;
	virtual void KeyCallback(int key, int scancode, int action, int mods) = 0;
	virtual void MouseButtonCallback(int button, int action, int mods) = 0;
	virtual void ScrollCallback(double yOffset) = 0;
    
    static void CursorPosCallbackDispatch(GLFWwindow* window, double xPos, double yPos)
    {
        m_CurrentView->CursorPosCallback(xPos, yPos);
    };
	static void KeyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        m_CurrentView->KeyCallback(key, scancode, action, mods);
    };
	static void MouseButtonCallbackDispatch(GLFWwindow* window, int button, int action, int mods)
    {
        m_CurrentView->MouseButtonCallback(button, action, mods);
    };
	static void ScrollCallbackDispatch(GLFWwindow* window, double xOffset, double yOffset)
    {
        m_CurrentView->ScrollCallback(yOffset);
    };
};

}