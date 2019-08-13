#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <memory>

#include "testbox.h"

namespace Jam3D {

class GLWindow
{
public:
    GLWindow(int width, int height, std::string title);
    void SetTest(std::shared_ptr<TestBox> test);
    void CursorPosCallback(double xPos, double yPos);

    int m_Width;
    int m_Height;
    std::string m_Title;
    GLFWwindow* m_Window;

    std::shared_ptr<TestBox> m_Test;
};

}