#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <memory>

#include "camera.h"

namespace Jam3D {

class GLWindow
{
public:
    GLWindow(int width, int height, std::string title);
    void InitGLFW();
    void InitCallbacks();
    void SetCamera(std::shared_ptr<Camera> camera);
    void CursorPosCallback(double xPos, double yPos);
    void ScrollCallback(double yOffset);
    void MouseButtonCallback(int button, int action, int mods);

    int m_Width;
    int m_Height;
    std::string m_Title;
    GLFWwindow* m_Window;

    std::shared_ptr<Camera> m_Camera;
};

}