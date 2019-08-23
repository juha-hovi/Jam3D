#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <memory>

#include "camera.h"
#include "vec2.h"

namespace Jam3D {

class GLWindow
{
public:
    GLWindow(int width, int height, const std::string& title);
    void InitGLFW();
    void InitCallbacks();
    void SetCamera(std::shared_ptr<Camera> camera);
    void CursorPosCallback(double xPos, double yPos);
    void KeyCallback(int key, int scancode, int action, int mods);
    void MouseButtonCallback(int button, int action, int mods);
    void ScrollCallback(double yOffset);

    int m_Width;
    int m_Height;
    std::string m_Title;
    GLFWwindow* m_Window;

    std::shared_ptr<Camera> m_Camera;
};

}