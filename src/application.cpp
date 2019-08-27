#include <memory>
#include <chrono>
#include <thread>
#include <functional>
#include <string>

#include "GL/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "application.h"
#include "log.h"
#include "view.h"

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
        Jam3D::Log::Error(std::string("******************************************************\n"
                                      "**GL ERROR**\n"
                                      "(Type): " + std::to_string(type) + "\n"
                                      "(Severity): " + std::to_string(severity) + "\n"
                                      "(Message): " + message + "\n"
                                      "******************************************************"));
    }
}

Application::Application()
    : m_ObjectDistance(500.0f), m_ObjectRotation(0.0f), m_ObjectLocation(0.0f), m_WindowWidth(1600), m_WindowHeight(900)
{
    if (!glfwInit())
        Jam3D::Log::Error("glfwInit failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_GLWindow = std::make_shared<GLWindow>(m_WindowWidth, m_WindowHeight, "Jam3D");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
        Jam3D::Log::Error("GLEW initialization failed!");

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLDebugCallback, 0);

    m_TestView = std::make_shared<TestView>(m_GLWindow);
    m_ObjectCreationView = std::make_shared<ObjectCreationView>(m_GLWindow);
    m_CurrentView = m_TestView;
    m_TestView->SetAsCurrent();
    InitCallbacks();
}

Application::~Application()
{
    glfwTerminate();
}

void Application::Run()
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
    ImGui_ImplGlfw_InitForOpenGL(m_GLWindow->m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // Loop until the window is closed by the user.
    while (!glfwWindowShouldClose(m_GLWindow->m_Window))  
    {
        frameStart = std::chrono::high_resolution_clock::now();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        DoTick();
        m_CurrentView->Render();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        
        {
            ImGui::Begin("Jam3D");
            ImGui::Text("Views:");
            if (ImGui::Button("Test"))
            {
                m_CurrentView = m_TestView;
                m_TestView->SetAsCurrent();
            }
            if (ImGui::Button("Object creation"))
            {
                m_CurrentView = m_ObjectCreationView;
                m_ObjectCreationView->SetAsCurrent();
            }
            ImGui::Text("========");
            ImGui::Text("Settings:");
            if (ImGui::Button("Toggle grid"))
                m_CurrentView->m_DrawPlanes = !(m_CurrentView->m_DrawPlanes); 
            if (ImGui::Button("Toggle axes"))
                m_CurrentView->m_DrawAxes = !(m_CurrentView->m_DrawAxes); 
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::End();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_GLWindow->m_Window);
        glfwPollEvents();

        frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameStart);
        sleepDuration = targetFrameDuration - frameDuration;
        std::this_thread::sleep_for(sleepDuration);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::InitCallbacks()
{
    glfwSetCursorPosCallback(m_GLWindow->m_Window, View::CursorPosCallbackDispatch);    
    glfwSetKeyCallback(m_GLWindow->m_Window, View::KeyCallbackDispatch);
    glfwSetMouseButtonCallback(m_GLWindow->m_Window, View::MouseButtonCallbackDispatch);
    glfwSetScrollCallback(m_GLWindow->m_Window, View::ScrollCallbackDispatch);
}

void Application::DoTick()
{
    float locationMult = 0.005f;
        m_ObjectLocation += locationMult;
        if (m_ObjectLocation > (2.0f * M_PI))
            m_ObjectLocation = 0.0f;

    float rotationMult = 0.8f;
    m_ObjectRotation += rotationMult;
    if (m_ObjectRotation >= 720.0f)
        m_ObjectRotation =  0.0f;

    if (m_CurrentView->m_Spheres.size() >= 1)
    {
        m_CurrentView->m_Spheres[0].SetPosition(Jam3D::Vec3<float>(std::sin(m_ObjectLocation) * m_ObjectDistance, -300.0f, std::cos(m_ObjectLocation) * m_ObjectDistance));
        m_CurrentView->m_Spheres[0].SetRotation(Jam3D::Vec3<float>(0.0f, m_ObjectRotation, 23.5f));        
    }

    if (m_CurrentView->m_Boxes.size() >= 1)
    {
        m_CurrentView->m_Boxes[0].m_Rotation.x = m_ObjectRotation;        
        m_CurrentView->m_Boxes[0].m_Rotation.y = m_ObjectRotation * 0.5f;
        m_CurrentView->m_Boxes[0].m_Rotation.z = 0.0f;
    }
}

}