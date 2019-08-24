#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <memory>

#include "vec2.h"

namespace Jam3D {

class GLWindow
{
public:
    GLWindow(int width, int height, const std::string& title);
    void InitGLFW();

    int m_Width;
    int m_Height;
    std::string m_Title;
    GLFWwindow* m_Window;

};

}