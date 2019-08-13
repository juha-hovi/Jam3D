#include "glwindow.h"

#include <iostream>

namespace Jam3D {

GLWindow::GLWindow(int width, int height, std::string title)
    : m_Width(width), m_Height(height), m_Title(title), m_Camera(nullptr)
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
    auto cursorPosFunc = [](GLFWwindow* window, double xPos, double yPos)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->CursorPosCallback(xPos, yPos);
    };
    glfwSetCursorPosCallback(m_Window, cursorPosFunc);

    glfwSetWindowUserPointer(m_Window, this);
    auto scrollFunc = [](GLFWwindow* window, double xOffset, double yOffset)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->ScrollCallback(yOffset);
    };
    glfwSetScrollCallback(m_Window, scrollFunc);
}

void GLWindow::SetCamera(std::shared_ptr<Camera> camera)
{
    m_Camera = camera;
}

void GLWindow::CursorPosCallback(double xPos, double yPos)
{
    m_Camera->CursorPosCallback(xPos, yPos);
}

void GLWindow::ScrollCallback(double yOffset)
{
    m_Camera->ScrollCallback(yOffset);
}

}