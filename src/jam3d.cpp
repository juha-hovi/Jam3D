#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <functional>

#include "GL/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "jam3d.h"
#include "testbox.h"

namespace Jam3D {

void GLAPIENTRY OpenGLDebugCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar* message,
                                    const void* userParam)
{
    // Only report errors
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        std::cout << "******************************************************" << "\n"
                << "**GL ERROR**" << "\n"
                << "(Type): " << type << "\n" 
                << "(Severity): " << severity << "\n" 
                << "(Message): " << message << "\n"
                << "******************************************************" << std::endl;
    }
}

/*  TODO:
        - Draw flowchart of OpenGL commands/tasks
        - Fix includes with GLEW/GLFW, removing glew breaks build but isn't needed in all files
        - Clean Camera class variables using vectors, add double vectors by templating math?
        - Figure out callbacks to member functions

        - Draw Boxes
            - Add support for multiple boxes (creation, deletion, modification)
        - Input?
        - Look into multiple viewport implementation
            - 4 views: general, xy-plane, xz-plane, yz-plane
                - Draw object by clicking mouse
*/

Jam3D::Jam3D()
    : m_Window(nullptr), m_Width(960), m_Height(540), m_Title("OpenGL Project")
{
    InitOpenGL();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLDebugCallback, 0);
}

Jam3D::~Jam3D()
{
    glfwTerminate();
}

void Jam3D::Run()
{
    // Setup timer
    int targetFps = 60;
    std::chrono::microseconds targetFrameDuration(1000000 / targetFps);
    std::chrono::microseconds sleepDuration;
    std::chrono::microseconds frameDuration;
    auto frameStart = std::chrono::high_resolution_clock::now();

    // Setup ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    TestBox test(m_Window);
    
    // Loop until the window is closed by the user.
    while (!glfwWindowShouldClose(m_Window))  
    {
        frameStart = std::chrono::high_resolution_clock::now();

        test.Render();
        test.RenderImGui();
        test.m_Camera->HandleMouse();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameStart);
        sleepDuration = targetFrameDuration - frameDuration;
        std::this_thread::sleep_for(sleepDuration);
    }
}

int Jam3D::InitOpenGL()
{
    if (!glfwInit())
        return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        std::cout << "Error: " << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(m_Window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }

    return 1;
}

}