#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "axes.h"
#include "box.h"
#include "sphere.h"
#include "lightsource.h"

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

class TestView
{
public:
    TestView(std::shared_ptr<GLWindow> window);

    void AddBox(Vec3 center, Vec3 dimensions, Vec3 rotation);
    void DeleteBox(int index);
    void AddSphere(float radius, Vec3 center, int sectorCount, int stackCount);
    void DeleteSphere(int index);
    void AddLightSource(unsigned int type, Vec3 position_or_direction, Vec3 color, float intensity);
    void DeleteLightSource(int index);

    void InitAxes();
    void InitRendering();

    void BufferBox(const Box& box);
    void BufferSphere(const Sphere& sphere);
    void SetLightSources();

    void Render();
    void RenderImGui();

private:

    Vec3 m_BoxCenter;
    Vec3 m_BoxDimensions;
    Vec3 m_BoxRotation;

    Vec3 m_SphereCenter;
    float m_SphereRadius;
    int m_SphereSectorCount;
    int m_SphereStackCount;

    float m_ObjectLocation;
    float m_ObjectDistance;
    Vec3 m_ObjectRotation;

    unsigned int m_LightType;
    Vec3 m_LightPosition;
    Vec3 m_LightColor;
    float m_LightIntensity;

    std::shared_ptr<GLWindow> m_Window;

    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_TextureBox;
    std::unique_ptr<Texture> m_TextureRGB;
    std::unique_ptr<Texture> m_TextureEarth;

    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<VertexBufferLayout> m_Layout;
    std::unique_ptr<IndexBuffer> m_IBO;

    std::unique_ptr<Axes> m_Axes;
    std::unique_ptr<VertexArray> m_VAO_axes;
    std::unique_ptr<VertexBuffer> m_VBO_axes;
    std::unique_ptr<VertexBufferLayout> m_Layout_axes;
    std::unique_ptr<IndexBuffer> m_IBO_axes;

    std::vector<Box> m_Boxes;
    std::vector<Sphere> m_Spheres;
    std::vector<LightSource> m_LightSources;

public:
    std::shared_ptr<Camera> m_Camera;
};

}