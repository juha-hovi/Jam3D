#include "glwindow.h"

#include <iostream>

namespace Jam3D {

GLWindow::GLWindow(int width, int height, std::string title)
    : m_Width(width), m_Height(height), m_Title(title), m_Test(nullptr)
{
    // Initialize GLFW
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        std::cout << "Error: " << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_Window);

    // Set up callbacks
    glfwSetWindowUserPointer(m_Window, this);
    auto func = [](GLFWwindow* window, double xPos, double yPos)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->CursorPosCallback(xPos, yPos);
    };
    glfwSetCursorPosCallback(m_Window, func);
}

void GLWindow::SetTest(std::shared_ptr<TestBox> test)
{
    m_Test = test;
}

void GLWindow::CursorPosCallback(double xPos, double yPos)
{
    m_Test->m_Camera->CursorPosCallback(xPos, yPos);
}

}