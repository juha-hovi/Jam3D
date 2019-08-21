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
#include "texture2d.h"
#include "texturecubemap.h"
#include "framebuffer.h"

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

    void AddBox(Jam3D::Vec3<float> center, Jam3D::Vec3<float> dimensions, Jam3D::Vec3<float> rotation);
    void DeleteBox(int index);
    void AddSphere(float radius, Jam3D::Vec3<float> center, int sectorCount, int stackCount);
    void DeleteSphere(int index);
    void AddLightSource(unsigned int type, Jam3D::Vec3<float> position_or_direction, Jam3D::Vec3<float> color, float intensity);
    void DeleteLightSource(int index);

    void InitAxes();
    void InitRendering();

    void BufferBox(const Box& box);
    void BufferSphere(const Sphere& sphere);
    void SetLightSources();
    void InitPointShadow();
    void UpdateShadowTransforms();
    void RenderPointShadow();
    void RenderScene();

    void UpdateModelMats();    
    void DoTick();

    void Render();
    void RenderImGui();

private:

    Jam3D::Vec3<float> m_BoxCenter;
    Jam3D::Vec3<float> m_BoxDimensions;
    Jam3D::Vec3<float> m_BoxRotation;

    Jam3D::Vec3<float> m_SphereCenter;
    float m_SphereRadius;
    int m_SphereSectorCount;
    int m_SphereStackCount;

    float m_ObjectLocation;
    float m_ObjectDistance;
    float m_ObjectRotation;

    unsigned int m_LightType;
    Jam3D::Vec3<float> m_LightPosition;
    Jam3D::Vec3<float> m_LightColor;
    float m_LightIntensity;

    const int m_ShadowWidth = 1024;
    const int m_ShadowHeight = 1024;
    std::unique_ptr<TextureCubeMap> m_TextureShadow;
    std::unique_ptr<FrameBuffer> m_FrameBuffer;

    float m_ShadowNearPlane;
    float m_ShadowFarPlane;
    glm::mat4 m_ShadowProjectionMatrix;
    std::vector<glm::mat4> m_ShadowTransforms;
    std::unique_ptr<Shader> m_Shader_shadow;
 
    std::shared_ptr<GLWindow> m_Window;

    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture2D> m_TextureBox;
    std::unique_ptr<Texture2D> m_TextureRGB;
    std::unique_ptr<Texture2D> m_TextureEarth;

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
    std::vector<glm::mat4> m_BoxModelMats;
    std::vector<Sphere> m_Spheres;
    std::vector<glm::mat4> m_SphereModelMats;
    std::vector<LightSource> m_LightSources;

public:
    std::shared_ptr<Camera> m_Camera;
};

}