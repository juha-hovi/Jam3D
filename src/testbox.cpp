#include "testbox.h"

TestBox::TestBox(Box::Vec3& corner0, Box::Vec3& corner1)
    : m_Rotation(0.0f), m_Increment(0.5f)
{
    m_Renderer = std::make_unique<Renderer>();
    m_Box = std::make_unique<Box>(corner0, corner1);
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(m_Box->m_Positions.data(), m_Box->m_PositionsSize * sizeof(float));
    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);

    m_VAO->AddBuffer(*m_VBO, *m_Layout);

    m_IBO = std::make_unique<IndexBuffer>(m_Box->m_Indices.data(), m_Box->m_IndicesSize);

    m_Shader = std::make_unique<Shader>("src/shaders/basic3d.shader");
    m_Shader->Bind();

    m_Texture = std::make_unique<Texture>("src/resources/tex_test.png");
    m_Shader->SetUniform1i("u_Texture", 0);

    proj = glm::mat4((glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 1.0f, 2000.0f)));
    view = glm::mat4((glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1000.0f)))); 
}

void TestBox::Update(Box::Vec3& corner0, Box::Vec3& corner1)
{
    m_Box->Update(corner0, corner1);
    m_VBO->UpdateBuffer(0, m_Box->m_PositionsSize * sizeof(float), m_Box->m_Positions.data());
}

void TestBox::Render()
{
    m_Renderer->Clear();
    glm::mat4 model(1.0f);
    glm::vec3 translation(m_Box->m_Center.x, m_Box->m_Center.y, m_Box->m_Center.z);
    model = glm::translate(model, translation);
    model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.5f, 1.0f, 0.0f));
    model = glm::translate(model, -translation);
    glm::mat4 mvp = proj * view * model;
    m_Texture->Bind();
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    m_Renderer->Draw(*m_VAO, *m_IBO, *m_Shader);

    if (m_Rotation >= 360.0f)
            m_Rotation = 0.0f;
    m_Rotation += m_Increment;
}