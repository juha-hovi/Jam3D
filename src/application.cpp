#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "box.h"
#include "testbox.h"

/*  TODO:
        - Change timer milli -> micro

        - Draw flowchart of OpenGL commands/tasks

        - Extend box class to offer 24 vertex option

        - Draw Boxes
            - Add support for multiple boxes (creation, deletion, modification)
        - Input
            - Camera control with keyboard
                - Left, right, up, down
                - Zoom in, zoom out
                - Pitch, yaw
            - Move something with mouse
                - Mouse1 click rotates object
                - Mouse1 click, hold, and drag rotates object
        - Look into multiple viewport implementation
            - 4 views: general, xy-plane, xz-plane, yz-plane
                - Draw object by clicking mouse
*/

void GLAPIENTRY openGLDebugCallback(GLenum source,
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

int main() 
{
    /////////////////////////////////
    /// Begin: GLWF & GLEW setup ////
    /////////////////////////////////
    GLFWwindow* window; 
    if (!glfwInit())
        return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "OpenGL Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openGLDebugCallback, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    std::cout << "GLFW & GLEW initialized" << "\n" << "GLEW: " << glewGetString(GLEW_VERSION) << "\n"
              << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    /////////////////////////////////
    //// End: GLWF & GLEW setup /////
    /////////////////////////////////

    // Setup timer
    int targetFps = 60;
    std::chrono::milliseconds targetFrameTime(1000 / targetFps);
    std::chrono::milliseconds sleepTime;
    std::chrono::milliseconds frameTime;
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
    while (!glfwWindowShouldClose(window))  
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

        glfwSwapBuffers(window);
        glfwPollEvents();

        frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
        sleepTime = targetFrameTime - frameTime;
        std::this_thread::sleep_for(sleepTime);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}