#pragma once

#include <vector>
#include <memory>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "viewport.h"
#include "glwindow.h"
#include "renderer.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexbufferlayout.h"
#include "shader.h"
#include "texture2d.h"
#include "texturecubemap.h"
#include "framebuffer.h"

#include "axes.h"
#include "box.h"
#include "sphere.h"
#include "lightsource.h"

#include "camera.h"

namespace Jam3D {

class View
{
public:
    View(std::shared_ptr<GLWindow> window);

    virtual void Render() = 0;    

protected:
    void AddBox(Jam3D::Vec3<float> center, Jam3D::Vec3<float> dimensions, Jam3D::Vec3<float> rotation);
    void AddSphere(float radius, Jam3D::Vec3<float> center, int sectorCount, int stackCount);
    void AddLightSource(unsigned int type, Jam3D::Vec3<float> position_or_direction, Jam3D::Vec3<float> color, float intensity);

    virtual void InitViewports() = 0;
    virtual void InitCameras() = 0;

    void InitRendering();
    void InitAxes();
    void InitPointShadow();

    void UpdateModelMats();
    void BufferShape(const Shape& shape);

    void UpdateShadowTransforms();
    void RenderPointShadow();
    void RenderScene(Camera &camera); 
    void SetLightSources();

    std::unique_ptr<VertexArray> m_VAOShape;
    std::unique_ptr<VertexBuffer> m_VBOShape;
    std::unique_ptr<VertexBufferLayout> m_LayoutShape;
    std::unique_ptr<IndexBuffer> m_IBOShape;

    std::unique_ptr<Axes> m_Axes;
    std::unique_ptr<VertexArray> m_VAOAxes;
    std::unique_ptr<VertexBuffer> m_VBOAxes;
    std::unique_ptr<VertexBufferLayout> m_LayoutAxes;
    std::unique_ptr<IndexBuffer> m_IBOAxes;

    const int m_ShadowWidth = 1024;
    const int m_ShadowHeight = 1024;
    std::unique_ptr<TextureCubeMap> m_TextureShadow;
    std::unique_ptr<FrameBuffer> m_FrameBuffer;

    float m_ShadowNearPlane;
    float m_ShadowFarPlane;
    glm::mat4 m_ShadowProjectionMatrix;
    std::vector<glm::mat4> m_ShadowTransforms;
    std::unique_ptr<Shader> m_ShaderShadow;

    std::unique_ptr<Shader> m_ShaderNormal;
    std::unique_ptr<Texture2D> m_TextureBox;
    std::unique_ptr<Texture2D> m_TextureRGB;
    std::unique_ptr<Texture2D> m_TextureEarth;

    std::shared_ptr<GLWindow> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    std::vector<Viewport> m_Viewports;

public:
    static std::vector<Box> m_Boxes;
    static std::vector<glm::mat4> m_BoxModelMats;
    static std::vector<Sphere> m_Spheres;
    static std::vector<glm::mat4> m_SphereModelMats;
    static std::vector<LightSource> m_LightSources;

    // Input handling
public:
    static View *m_CurrentView;
    virtual void SetAsCurrent() { m_CurrentView = this; };

protected:
    virtual void CursorPosCallback(double xPos, double yPos) = 0;
	virtual void KeyCallback(int key, int scancode, int action, int mods) = 0;
	virtual void MouseButtonCallback(int button, int action, int mods) = 0;
	virtual void ScrollCallback(double yOffset) = 0;

public:
    static void CursorPosCallbackDispatch(GLFWwindow* window, double xPos, double yPos)
    {
        m_CurrentView->CursorPosCallback(xPos, yPos);
    };
	static void KeyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        m_CurrentView->KeyCallback(key, scancode, action, mods);
    };
	static void MouseButtonCallbackDispatch(GLFWwindow* window, int button, int action, int mods)
    {
        m_CurrentView->MouseButtonCallback(button, action, mods);
    };
	static void ScrollCallbackDispatch(GLFWwindow* window, double xOffset, double yOffset)
    {
        m_CurrentView->ScrollCallback(yOffset);
    };
};

}