#include "glwindow.h"
#include "log.h"


namespace Jam3D {

GLWindow::GLWindow(int width, int height, const std::string& title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    InitGLFW();
    //InitCallbacks();
}

void GLWindow::InitGLFW()
{
     m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        Jam3D::Log::Error("GLFW window creation failed!");
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_Window);
}

}