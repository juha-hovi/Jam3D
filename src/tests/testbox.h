#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "box.h"
#include "axes.h"

#include "camera.h"
#include "renderer.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexbufferlayout.h"
#include "shader.h"
#include "texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace Jam3D {

class TestBox
{
public:
    TestBox(GLFWwindow* window);
    ~TestBox();

    void Update();
    void Render();
    void RenderImGui();

    float m_Rotation;
    float m_Increment;

private:
    GLFWwindow* m_Window;

    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Box> m_Box;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<VertexBufferLayout> m_Layout;
    std::unique_ptr<IndexBuffer> m_IBO;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

    std::unique_ptr<Axes> m_Axes;
    std::unique_ptr<VertexArray> m_VAO_axes;
    std::unique_ptr<VertexBuffer> m_VBO_axes;
    std::unique_ptr<VertexBufferLayout> m_Layout_axes;
    std::unique_ptr<IndexBuffer> m_IBO_axes;

    glm::mat4 proj;
    glm::mat4 view;

public:
    std::shared_ptr<Camera> m_Camera;
};

}