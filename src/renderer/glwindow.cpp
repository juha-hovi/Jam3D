#include "glwindow.h"

#include <iostream>

namespace Jam3D {

GLWindow::GLWindow(int width, int height, std::string title)
    : m_Width(width), m_Height(height), m_Title(title), m_Camera(nullptr)
{
    InitGLFW();
    InitCallbacks();
}

void GLWindow::InitGLFW()
{
     m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        std::cout << "Error: " << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_Window);
}

void GLWindow::InitCallbacks()
{
    glfwSetWindowUserPointer(m_Window, this);
    auto cursorPosFunc = [](GLFWwindow* window, double xPos, double yPos)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->CursorPosCallback(xPos, yPos);
    };
    glfwSetCursorPosCallback(m_Window, cursorPosFunc);

    glfwSetWindowUserPointer(m_Window, this);
    auto keyFunc = [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->KeyCallback(key, scancode, action, mods);
    };
    glfwSetKeyCallback(m_Window, keyFunc);

    glfwSetWindowUserPointer(m_Window, this);
    auto mouseButtonFunc = [](GLFWwindow* window, int button, int action, int mods)
    {
        static_cast<GLWindow*>(glfwGetWindowUserPointer(window))->MouseButtonCallback(button, action, mods);
    };
    glfwSetMouseButtonCallback(m_Window, mouseButtonFunc);
}

void GLWindow::SetCamera(std::shared_ptr<Camera> camera)
{
    m_Camera = camera;
}

void GLWindow::CursorPosCallback(double xPos, double yPos)
{
    m_Camera->CursorPosCallback(xPos, yPos);
}

void GLWindow::KeyCallback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W || 
        key == GLFW_KEY_A || 
        key == GLFW_KEY_S || 
        key == GLFW_KEY_D)
    {
        m_Camera->KeyCallback(key, scancode, action, mods);
    }
}

void GLWindow::MouseButtonCallback(int button, int action, int mods)
{
    m_Camera->MouseButtonCallback(button, action, mods);
}

}