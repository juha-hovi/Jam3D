#pragma once

#include "box.h"
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

class TestBox
{
public:
    TestBox();
    ~TestBox();

    void Update();
    void Render();
    void RenderImGui();

    float m_Rotation;
    float m_Increment;

private:
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Box> m_Box;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<VertexBufferLayout> m_Layout;
    std::unique_ptr<IndexBuffer> m_IBO;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

    glm::mat4 proj;
    glm::mat4 view;
};