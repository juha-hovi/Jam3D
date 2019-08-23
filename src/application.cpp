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
{
    if (!glfwInit())
        Jam3D::Log::Error("glfwInit failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_GLWindow = std::make_shared<GLWindow>(1600, 900, "Jam3D");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
        Jam3D::Log::Error("GLEW initialization failed!");

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLDebugCallback, 0);
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

    m_TestView = std::make_shared<TestView>(m_GLWindow);
    m_ObjectCreationView = std::make_shared<ObjectCreationView>(m_GLWindow);
    m_CurrentView = m_TestView;
    m_GLWindow->SetCamera(m_TestView->m_Camera);
    
    // Loop until the window is closed by the user.
    while (!glfwWindowShouldClose(m_GLWindow->m_Window))  
    {
        frameStart = std::chrono::high_resolution_clock::now();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        m_CurrentView->Render();
        
        {
            ImGui::Begin("Views");
            if (ImGui::Button("Test"))
                m_CurrentView = m_TestView;
            if (ImGui::Button("Object creation"))
                m_CurrentView = m_ObjectCreationView;
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

}