#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "box.h"
#include "testbox.h"

/*  TODO:
        - Change Application into class

        - Draw flowchart of OpenGL commands/tasks

        - Add support for 24 vertex box
        - Fix/check vertex order, front face -> counter clockwise vertices
        - Enable facet culling: glEnable(GL_CULL_FACE)

        - Draw Boxes
            - Add support for multiple boxes (creation, deletion, modification)
        - Input
            - Camera control with keyboard
                - Camera class
                - Left, right, up, down
                - Zoom in, zoom out
                - Pitch, yaw
                - Rotation
            - Move something with mouse
                - Mouse1 click rotates object
                - Mouse1 click, hold, and drag rotates object
        - Look into multiple viewport implementation
            - 4 views: general, xy-plane, xz-plane, yz-plane
                - Draw object by clicking mouse
*/

Application::Application()
    : m_Window(nullptr), m_Width(960), m_Height(540), m_Title("OpenGL Project")
{
    InitOpenGL();

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
    std::chrono::microseconds targetFrameTime(1000 / targetFps);
    std::chrono::microseconds sleepTime;
    std::chrono::microseconds frameTime;
    auto frameStart = std::chrono::high_resolution_clock::now();

    // Setup box test
    Box::Vec3 corner0(-200.0f, -100.0f, -100.0f);
    Box::Vec3 corner1(0.0f, 100.0f, 100.0f);
    Box::Vec3 corner0New(-200.0f, -100.0f, -100.0f);
    Box::Vec3 corner1New(0.0f, 100.0f, 100.0f);
    TestBox testBox(corner0, corner1);

    // Setup ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // Loop until the window is closed by the user.
    while (!glfwWindowShouldClose(m_Window))  
    {
        frameStart = std::chrono::high_resolution_clock::now();

        ////////////////////////////////////////////////
        testBox.Render();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("OpenGL Project");
            ImGui::SliderFloat("Corner 0: x", &corner0New.x, -500.0f, 500.0f);
            ImGui::SliderFloat("Corner 0: y", &corner0New.y, -500.0f, 500.0f);
            ImGui::SliderFloat("Corner 0: z", &corner0New.z, -500.0f, 500.0f);
            ImGui::SliderFloat("Corner 1: x", &corner1New.x, -500.0f, 500.0f);
            ImGui::SliderFloat("Corner 1: y", &corner1New.y, -500.0f, 500.0f);
            ImGui::SliderFloat("Corner 1: z", &corner1New.z, -500.0f, 500.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        if (corner0New != corner0 || corner1New != corner1)
        {
            corner0 = corner0New;
            corner1 = corner1New;
            testBox.Update(corner0New, corner1New);
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ////////////////////////////////////////////////

        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        frameTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameStart);
        sleepTime = targetFrameTime - frameTime;
        std::this_thread::sleep_for(sleepTime);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int Application::InitOpenGL()
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

void GLAPIENTRY Application::OpenGLDebugCallback(GLenum source,
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
