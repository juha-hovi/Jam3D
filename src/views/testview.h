#pragma once

#include "view.h"

#include "axes.h"
#include "shape.h"

#include "perspectivecamera.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexbufferlayout.h"
#include "shader.h"
#include "texture2d.h"
#include "texturecubemap.h"
#include "framebuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Jam3D {

class TestView : public View
{
public:
    TestView(std::shared_ptr<GLWindow> window);
    void Render() override;

    void CursorPosCallback(double xPos, double yPos) override;
	void KeyCallback(int key, int scancode, int action, int mods) override;
	void MouseButtonCallback(int button, int action, int mods) override;
	void ScrollCallback(double yOffset) override;

private:
    void InitAxes();
    void InitRendering();

    void SetLightSources();
    void InitPointShadow();
    void UpdateShadowTransforms();
    void RenderPointShadow();
    void RenderScene();

    void UpdateModelMats();    

    void RenderImGui();

public:
    std::shared_ptr<PerspectiveCamera> m_Camera;

private:
    Jam3D::Vec3<float> m_BoxCenter;
    Jam3D::Vec3<float> m_BoxDimensions;
    Jam3D::Vec3<float> m_BoxRotation;

    Jam3D::Vec3<float> m_SphereCenter;
    float m_SphereRadius;
    int m_SphereSectorCount;
    int m_SphereStackCount;

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
    std::unique_ptr<Shader> m_ShaderShadow;

    int m_NormalViewportIndex;
    int m_ShadowViewportIndex;
 
    std::unique_ptr<Shader> m_ShaderNormal;
    std::unique_ptr<Texture2D> m_TextureBox;
    std::unique_ptr<Texture2D> m_TextureRGB;
    std::unique_ptr<Texture2D> m_TextureEarth;
};

}