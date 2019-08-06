#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "shader.h"
#include "renderer.h"
#include "box.h"
#include "texture.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"
#include "indexbuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/*  TODO:
        - Draw flowchart of OpenGL commands/tasks

        - Draw Boxes
            - Draw anything
            - Add a way to input box corners (cin.get()?)
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
    std::cout << "[GL CALLBACK]: " << (type == GL_DEBUG_TYPE_ERROR ? "**GL ERROR**" : "")
              << "(Type): " << type << "\n" 
              << "(Severity): " << severity << "\n" 
              << "(Message): " << message << std::endl;
}

int main() 
{
    /////////////////////////////////
    /// Begin: GLWF & GLEW setup ////
    /////////////////////////////////
    GLFWwindow* window; 
    if (!glfwInit())
        return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openGLDebugCallback, 0);

    std::cout << "GLFW & GLEW initialized" << "\n" << "GLEW: " << glewGetString(GLEW_VERSION) << "\n"
              << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    /////////////////////////////////
    //// End: GLWF & GLEW setup /////
    /////////////////////////////////

    Renderer renderer;
    Box box(Box::Vec3(-200, -200, -200), Box::Vec3(200, 200, 200));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    VertexArray VAO;
    VertexBuffer VBO(box.m_Positions, box.m_PositionsSize * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    VAO.AddBuffer(VBO, layout);

    IndexBuffer IBO(box.m_Indices, box.m_IndicesSize);

    Shader shader("src/shaders/basic3d.shader");
    shader.Bind();

    Texture texture("src/resources/image.png");
    shader.SetUniform1i("u_Texture", 0);

    glm::mat4 proj(glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 2000.0f));
    glm::mat4 view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1000.0f)));


    // Loop until the window is closed by the user.
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.Clear();
        
        
        texture.Bind();
        glm::mat4 mvp = proj * view;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(VAO, IBO, shader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}