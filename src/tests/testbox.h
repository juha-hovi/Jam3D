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

#include "glwindow.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>

namespace Jam3D {

class TestBox
{
public:
    TestBox(std::shared_ptr<GLWindow> window);

    void AddBox(Vec3 corner0, Vec3 corner1);
    void DeleteBox(int index);
    void InitAxes();
    void InitRendering();

    void BufferBox(const Box& box);
    void Render();
    void RenderImGui();

private:
    int m_PositionsSize;
    int m_IndicesSize;

    Vec3 m_Corner0;
    Vec3 m_Corner1;

    std::shared_ptr<GLWindow> m_Window;

    std::unique_ptr<Renderer> m_Renderer;
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

    std::vector<Box> m_Boxes;
    std::vector<float> m_Positions;
    std::vector<unsigned int> m_Indices;

    glm::mat4 proj;
    glm::mat4 view;

public:
    std::shared_ptr<Camera> m_Camera;
};

}