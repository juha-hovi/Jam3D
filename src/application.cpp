#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "box.h"
#include "testbox.h"

/*  TODO:
        - Draw flowchart of OpenGL commands/tasks

        - Extend box class to offer 24 vertex option

        - Draw Boxes
            - Add a way to input box corners
            - Add support for multiple boxes
        - Input
            - Make ImGui work
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

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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

    TestBox testBox(Box::Vec3(-100.0f, -100.0f, -100.0f), Box::Vec3(200.0f, 200.0f, 200.0f));

    // Loop until the window is closed by the user.
    while (!glfwWindowShouldClose(window))  
    {
        frameStart = std::chrono::high_resolution_clock::now();

        testBox.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
        sleepTime = targetFrameTime - frameTime;
        std::this_thread::sleep_for(sleepTime);
    }

    glfwTerminate();
    return 0;
}