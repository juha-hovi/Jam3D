#include "objectcreationview.h"
#include "vec4.h"
#include "mathhelper.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

namespace Jam3D {

ObjectCreationView::ObjectCreationView(std::shared_ptr<GLWindow> window) 
    : View(window), m_UpperLeftViewportIndex(-1), m_UpperRightViewportIndex(-1), m_LowerLeftViewportIndex(-1),
    m_LowerRightViewportIndex(-1), m_ShadowViewportIndex(-1),
    m_MouseRightPressedUpperLeft(false), m_MouseLeftPressedUpperLeft(false), m_MouseLeftPressedUpperRight(false),
    m_MouseLeftPressedLowerLeft(false), m_MouseLeftPressedLowerRight(false),
    m_MouseLeftPressedBetweenLeftRight(false), m_MouseLeftPressedBetweenUpperLower(false),
    m_MouseLeftPressedTempBoxXMinusMargin(false), m_MouseLeftPressedTempBoxXPlusMargin(false),
    m_MouseLeftPressedTempBoxYMinusMargin(false), m_MouseLeftPressedTempBoxYPlusMargin(false),
    m_MouseLeftPressedTempBoxZMinusMargin(false), m_MouseLeftPressedTempBoxZPlusMargin(false),
    m_MouseLeftPressLocation(glm::vec3(0.0f, 0.0f, 0.0f)), m_CurrentTool(ObjectCreationView::MOVE),
    m_TempBoxCenterOriginal(0.0f, 0.0f, 0.0f), m_TempBoxDimensionsOriginal(0.0f, 0.0f, 0.0f),
    m_ViewportDivider(2), m_ViewportMiddle(0.0f, 0.0f), m_ViewportmiddleOriginal(0.0f, 0.0f),
    m_OrthoCamDimMultip(1.5f)
{
    InitViewports();
    InitCameras();
    InitViewportBorders();
}

void ObjectCreationView::InitViewports()
{
    m_UpperLeftViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(0, m_Window->m_Height / 2 + m_ViewportDivider, m_Window->m_Width / 2 - m_ViewportDivider, m_Window->m_Height / 2 - m_ViewportDivider));
    m_UpperRightViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(m_Window->m_Width / 2 + m_ViewportDivider, m_Window->m_Height / 2 + m_ViewportDivider, m_Window->m_Width / 2 - m_ViewportDivider, m_Window->m_Height / 2 - m_ViewportDivider));
    m_LowerLeftViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(0, 0, m_Window->m_Width / 2 - m_ViewportDivider, m_Window->m_Height / 2 - m_ViewportDivider));
    m_LowerRightViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Jam3D::Vec4<int>(m_Window->m_Width / 2 + m_ViewportDivider, 0, m_Window->m_Width / 2 - m_ViewportDivider, m_Window->m_Height / 2 - m_ViewportDivider));
    m_ShadowViewportIndex = m_Viewports.size();
    m_Viewports.push_back(Viewport(Jam3D::Vec4<int>(0, 0, m_ShadowWidth, m_ShadowHeight)));

    m_ViewportMiddle.x = m_Window->m_Width / 2;
    m_ViewportMiddle.y = m_Window->m_Height / 2;
    m_ViewportmiddleOriginal = m_ViewportMiddle;
}

void ObjectCreationView::UpdateViewports()
{
    m_Viewports[m_UpperLeftViewportIndex].m_Corners.x1 = m_ViewportMiddle.x - m_ViewportDivider;
    m_Viewports[m_UpperLeftViewportIndex].m_Corners.y0 = m_ViewportMiddle.y + m_ViewportDivider;

    m_Viewports[m_UpperRightViewportIndex].m_Corners.x0 = m_ViewportMiddle.x + m_ViewportDivider;
    m_Viewports[m_UpperRightViewportIndex].m_Corners.y0 = m_ViewportMiddle.y + m_ViewportDivider;

    m_Viewports[m_LowerLeftViewportIndex].m_Corners.x1 = m_ViewportMiddle.x - m_ViewportDivider;
    m_Viewports[m_LowerLeftViewportIndex].m_Corners.y1 = m_ViewportMiddle.y - m_ViewportDivider;

    m_Viewports[m_LowerRightViewportIndex].m_Corners.x0 = m_ViewportMiddle.x + m_ViewportDivider;
    m_Viewports[m_LowerRightViewportIndex].m_Corners.y1 = m_ViewportMiddle.y - m_ViewportDivider;

    m_Viewports[m_UpperLeftViewportIndex].m_Corners.y1 = m_Window->m_Height - m_ViewportMiddle.y - m_ViewportDivider;
    m_Viewports[m_UpperRightViewportIndex].m_Corners.x1 = m_Window->m_Width - m_ViewportMiddle.x - m_ViewportDivider;
    m_Viewports[m_UpperRightViewportIndex].m_Corners.y1 = m_Window->m_Height - m_ViewportMiddle.y - m_ViewportDivider;
    m_Viewports[m_LowerRightViewportIndex].m_Corners.x1 = m_Window->m_Width - m_ViewportMiddle.x - m_ViewportDivider;
}

void ObjectCreationView::InitCameras()
{
    Camera::CameraBaseProperties properties;
    float fov = 45.0f;
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    properties.near = 1.0f;
    properties.far = 5000.0f;
    properties.position = glm::vec3(1000.0f, 1000.0f, -1000.0f);
    properties.target = glm::vec3(0.0f, 0.0f, 0.0f);
    properties.pitch = 35.0f;
    properties.yaw = -45.0f;
    properties.windowDim = Jam3D::Vec2<float>({(float)m_Window->m_Width, (float)m_Window->m_Height});
    properties.window = m_Window;
    m_UpperLeftCamera = std::make_shared<PerspectiveCamera>(fov, target, properties);
  
    properties.pitch = 90.0f;
    properties.position = glm::vec3(0.0f, 2000.0f, 0.0f);
    m_UpperRightCamera = std::make_shared<OrthoCamera>(0.0f, 1.0f, 0.0f, 1.0f, properties);

    properties.pitch = 0.0f;
    properties.position = glm::vec3(0.0f, 0.0f, -2000.0f);
    m_LowerLeftCamera = std::make_shared<OrthoCamera>(0.0f, 1.0f, 0.0f, 1.0f, properties);

    properties.position = glm::vec3(2000.0f, 0.0f, 0.0f);
    m_LowerRightCamera = std::make_shared<OrthoCamera>(0.0f, 1.0f, 0.0f, 1.0f, properties);

    UpdateCameraProj();
}

void ObjectCreationView::Render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  

    UpdateModelMats();
    UpdateCameraProj();
    m_UpperLeftCamera->Update();
    m_UpperRightCamera->Update();
    m_LowerLeftCamera->Update();
    m_LowerRightCamera->Update();

    // Render shadows
    m_Viewports[m_ShadowViewportIndex].Use();
    RenderPointShadow();

    m_Renderer->Clear();

    // Render overall window
    m_Viewports[m_UpperLeftViewportIndex].Use();
    RenderScene(*m_UpperLeftCamera, true);
    RenderMisc(*m_UpperLeftCamera, m_DrawAxes, m_DrawPlanes, false, false);

    // Render xz
    m_Viewports[m_UpperRightViewportIndex].Use();
    RenderScene(*m_UpperRightCamera, false);
    RenderMisc(*m_UpperRightCamera, m_DrawAxes, m_DrawPlanes, false, false);

    // Render xy
    m_Viewports[m_LowerLeftViewportIndex].Use();
    RenderScene(*m_LowerLeftCamera, false);
    RenderMisc(*m_LowerLeftCamera, m_DrawAxes, false, m_DrawPlanes, false);

    // Render yz
    m_Viewports[m_LowerRightViewportIndex].Use();
    RenderScene(*m_LowerRightCamera, false);
    RenderMisc(*m_LowerRightCamera, m_DrawAxes, false, false, m_DrawPlanes);

    RenderViewportBorders();
    
    RenderImGui();
}

void ObjectCreationView::RenderImGui()
{
    {
        ImGui::Begin("Draw box");
        if (!m_TempBox)
        {
            if (ImGui::Button("Start"))
                StartDrawing();
        }
        else
        {
            if (ImGui::Button("Reset"))
                StartDrawing();
        }
        if (ImGui::Button("Save"))
            SaveTempBox();
        if (ImGui::Button("Cancel"))
            CancelDrawing();
        ImGui::Text("======");
        ImGui::Text("Tools:");
        if (ImGui::Button("Move"))
            m_CurrentTool = ObjectCreationView::MOVE;
        if (ImGui::Button("Stretch"))
            m_CurrentTool = ObjectCreationView::STRECH;
        if (ImGui::Button("Draw"))
            m_CurrentTool = ObjectCreationView::DRAW;
        ImGui::End();
    }

    {
        ImGui::Begin("Camera");
        ImGui::SliderFloat("Zoom", &m_OrthoCamDimMultip, 0.2f, 2.0f);
        ImGui::End();
    }
}

void ObjectCreationView::StartDrawing()
{
    Jam3D::Vec3<float> center(0.0f, 0.0f, 0.0f);
    Jam3D::Vec3<float> dimension(400.0f, 400.0f, 400.0f);
    Jam3D::Vec3<float> rotation(0.0f, 0.0f, 0.0f);

    m_TempBox = std::make_unique<Box>(center, dimension, rotation);
    m_TempBoxCenterOriginal = center;
    m_TempBoxDimensionsOriginal = dimension;
}

void ObjectCreationView::SaveTempBox()
{
    AddBox(m_TempBox->m_Center, m_TempBox->m_Dimensions, m_TempBox->m_Rotation);
    m_TempBox = nullptr;
}

void ObjectCreationView::CancelDrawing()
{
    m_TempBox = nullptr;
}

void ObjectCreationView::UpdateTempBox(OrthoCamera& camera, Viewport& viewport, double xPos, double yPos, bool x, bool y, bool z)
{
    glm::vec3 worldCoords = glm::unProject(glm::vec3(xPos, yPos, 1.0f), camera.m_ViewMatrix, camera.m_ProjectionMatrix, Jam3D::MathHelper::Vec4ToGlm(viewport.m_Corners));

    if (m_CurrentTool == ObjectCreationView::DRAW)
        DrawTempBox(worldCoords, x, y, z);
    else if (m_CurrentTool == ObjectCreationView::STRECH)
        StrechTempBox(worldCoords, x, y, z);
    else if (m_CurrentTool == ObjectCreationView::MOVE)
        MoveTempBox(worldCoords, x, y, z);
}

void ObjectCreationView::DrawTempBox(glm::vec3 worldCoords, bool x, bool y, bool z)
{
    if (x)
    {
        double xCenter = 0.0;
        double xDim = worldCoords.x - m_MouseLeftPressLocation.x;
        if (worldCoords.x > m_MouseLeftPressLocation.x)
            xCenter = worldCoords.x - xDim / 2;
        else
        {
            xDim = -xDim;
            xCenter = m_MouseLeftPressLocation.x - xDim / 2;
        }
        m_TempBox->m_Dimensions.x = xDim;
        m_TempBox->m_Center.x = xCenter;
    }

    if (y)
    {
        double yCenter = 0.0;
        double yDim = worldCoords.y - m_MouseLeftPressLocation.y;
        if (worldCoords.y > m_MouseLeftPressLocation.y)
            yCenter = worldCoords.y - yDim / 2;
        else 
        {
            yDim = -yDim;
            yCenter = m_MouseLeftPressLocation.y - yDim / 2;
        }
        m_TempBox->m_Dimensions.y = yDim;
        m_TempBox->m_Center.y = yCenter;
    }

    if (z)
    {
        double zCenter = 0.0;
        double zDim = worldCoords.z - m_MouseLeftPressLocation.z;
        if (worldCoords.z > m_MouseLeftPressLocation.z)
            zCenter = worldCoords.z - zDim / 2;
        else 
        {
            zDim = -zDim;
            zCenter = m_MouseLeftPressLocation.z - zDim / 2;
        }
        m_TempBox->m_Dimensions.z = zDim;
        m_TempBox->m_Center.z = zCenter;
    }

    m_TempBox->Update();
}

void ObjectCreationView::StrechTempBox(glm::vec3 worldCoords, bool x, bool y, bool z)
{
    float minBoxDim = 50;

    if (x)
    {
        float xDiff = worldCoords.x - m_MouseLeftPressLocation.x;
        if (m_MouseLeftPressedTempBoxXMinusMargin)
        {
            if (m_TempBoxDimensionsOriginal.x + xDiff > minBoxDim)
            {
                m_TempBox->m_Dimensions.x = m_TempBoxDimensionsOriginal.x + xDiff;
                m_TempBox->m_Center.x = m_TempBoxCenterOriginal.x + xDiff / 2;
            }
        }
        else if (m_MouseLeftPressedTempBoxXPlusMargin)
        {
            if (m_TempBoxDimensionsOriginal.x - xDiff > minBoxDim)
            {
                m_TempBox->m_Dimensions.x = m_TempBoxDimensionsOriginal.x - xDiff;
                m_TempBox->m_Center.x = m_TempBoxCenterOriginal.x + xDiff / 2;
            }
        }
    }

    if (y)
    {
        double yDiff = worldCoords.y - m_MouseLeftPressLocation.y;
        if (m_MouseLeftPressedTempBoxYMinusMargin)
        {
            if (m_TempBoxDimensionsOriginal.y - yDiff > minBoxDim)
            {
                m_TempBox->m_Dimensions.y = m_TempBoxDimensionsOriginal.y - yDiff;
                m_TempBox->m_Center.y = m_TempBoxCenterOriginal.y + yDiff / 2;
            }
        }
        else if (m_MouseLeftPressedTempBoxYPlusMargin)
        {
            if (m_TempBoxDimensionsOriginal.y + yDiff > minBoxDim)
            {
                m_TempBox->m_Dimensions.y = m_TempBoxDimensionsOriginal.y + yDiff;
                m_TempBox->m_Center.y = m_TempBoxCenterOriginal.y + yDiff / 2;
            }
        }
    }

    if (z)
    {
        double zDiff = worldCoords.z - m_MouseLeftPressLocation.z;
        if (m_MouseLeftPressedTempBoxZMinusMargin)
        {
            if (m_TempBoxDimensionsOriginal.z - zDiff > minBoxDim)
            {
                m_TempBox->m_Dimensions.z = m_TempBoxDimensionsOriginal.z - zDiff;
                m_TempBox->m_Center.z = m_TempBoxCenterOriginal.z + zDiff / 2;
            }
        }
        else if (m_MouseLeftPressedTempBoxZPlusMargin)
        {
            if (m_TempBoxDimensionsOriginal.z + zDiff > minBoxDim)
            {
                m_TempBox->m_Dimensions.z = m_TempBoxDimensionsOriginal.z + zDiff;
                m_TempBox->m_Center.z = m_TempBoxCenterOriginal.z + zDiff / 2;
            }
        }
    }

    m_TempBox->Update();
}

void ObjectCreationView::MoveTempBox(glm::vec3 worldCoords, bool x, bool y, bool z)
{
    if (x)
    {
        double xDiff = worldCoords.x - m_MouseLeftPressLocation.x;
        m_TempBox->m_Center.x = m_TempBoxCenterOriginal.x + xDiff;
    }

    if (y)
    {
        double yDiff = worldCoords.y - m_MouseLeftPressLocation.y;
        m_TempBox->m_Center.y = m_TempBoxCenterOriginal.y + yDiff;
    }

    if (z)
    {
        double zDiff = worldCoords.z - m_MouseLeftPressLocation.z;
        m_TempBox->m_Center.z = m_TempBoxCenterOriginal.z + zDiff;
    }
}

void ObjectCreationView::ResizeViewports(double pos, bool x, bool y)
{
    if (x)
    {
        double diff = pos - m_MouseLeftPressLocation.x;
        m_ViewportMiddle.x = m_ViewportmiddleOriginal.x + diff;
    }

    if (y)
    {
        double diff = pos - m_MouseLeftPressLocation.y;
        m_ViewportMiddle.y = m_ViewportmiddleOriginal.y + diff;
    }

    UpdateViewports();
    InitViewportBorders();
    UpdateCameraProj();
}

void ObjectCreationView::UpdateCameraProj()
{
    m_UpperLeftCamera->UpdateProjMat((float)m_Viewports[m_UpperLeftViewportIndex].m_Corners.x1 / (float)m_Viewports[m_UpperLeftViewportIndex].m_Corners.y1);

    float left = -m_Viewports[m_UpperRightViewportIndex].m_Corners.x1 / m_OrthoCamDimMultip;
    float right = m_Viewports[m_UpperRightViewportIndex].m_Corners.x1 / m_OrthoCamDimMultip;
    float bottom = -m_Viewports[m_UpperRightViewportIndex].m_Corners.y1 / m_OrthoCamDimMultip;
    float top = m_Viewports[m_UpperRightViewportIndex].m_Corners.y1 / m_OrthoCamDimMultip;     
    m_UpperRightCamera->UpdateProjMat(left, right, bottom, top);

    left = -m_Viewports[m_LowerLeftViewportIndex].m_Corners.x1 / m_OrthoCamDimMultip;
    right = m_Viewports[m_LowerLeftViewportIndex].m_Corners.x1 / m_OrthoCamDimMultip;
    bottom = -m_Viewports[m_LowerLeftViewportIndex].m_Corners.y1 / m_OrthoCamDimMultip;
    top = m_Viewports[m_LowerLeftViewportIndex].m_Corners.y1 / m_OrthoCamDimMultip; 
    m_LowerLeftCamera->UpdateProjMat(left, right, bottom, top);

    left = -m_Viewports[m_LowerRightViewportIndex].m_Corners.x1 / m_OrthoCamDimMultip;
    right = m_Viewports[m_LowerRightViewportIndex].m_Corners.x1 / m_OrthoCamDimMultip;
    bottom = -m_Viewports[m_LowerRightViewportIndex].m_Corners.y1 / m_OrthoCamDimMultip;
    top = m_Viewports[m_LowerRightViewportIndex].m_Corners.y1 / m_OrthoCamDimMultip; 
    m_LowerRightCamera->UpdateProjMat(left, right, bottom, top);
}

void ObjectCreationView::IsInMargin(glm::vec3 worldCoords, bool x, bool y, bool z)
{
    double marginRatio = 0.15;

    if (x && z)
    {
        double xMargin = m_TempBox->m_Dimensions.x * marginRatio;
        double xMinus = m_TempBox->m_Center.x + m_TempBox->m_Dimensions.x / 2;
        double xPlus = m_TempBox->m_Center.x - m_TempBox->m_Dimensions.x / 2;
        double zMargin = m_TempBox->m_Dimensions.z * marginRatio;
        double zMinus = m_TempBox->m_Center.z - m_TempBox->m_Dimensions.z / 2;
        double zPlus = m_TempBox->m_Center.z + m_TempBox->m_Dimensions.z / 2;

        Jam3D::Vec4<double> xMinusRect(xMinus - xMargin, zMinus, xMargin * 2, m_TempBox->m_Dimensions.z);
        Jam3D::Vec4<double> xPlusRect(xPlus - xMargin, zMinus, xMargin * 2, m_TempBox->m_Dimensions.z);
        Jam3D::Vec4<double> zMinusRect(xPlus, zMinus - zMargin, m_TempBox->m_Dimensions.x, zMargin * 2);
        Jam3D::Vec4<double> zPlusRect(xPlus, zPlus - zMargin, m_TempBox->m_Dimensions.x, zMargin * 2);

        if (MathHelper::IsInRect(worldCoords.x, worldCoords.z, xMinusRect))
        {
            m_MouseLeftPressedTempBoxXMinusMargin = true;
        }

        else if (MathHelper::IsInRect(worldCoords.x, worldCoords.z, xPlusRect))
        {
            m_MouseLeftPressedTempBoxXPlusMargin = true;
        }

        if (MathHelper::IsInRect(worldCoords.x, worldCoords.z, zMinusRect))
        {
            m_MouseLeftPressedTempBoxZMinusMargin = true;
        }

        else if (MathHelper::IsInRect(worldCoords.x, worldCoords.z, zPlusRect))
        {
            m_MouseLeftPressedTempBoxZPlusMargin = true;
        }
    }

    else if (x && y)
    {
        double xMargin = m_TempBox->m_Dimensions.x * marginRatio;
        double xMinus = m_TempBox->m_Center.x + m_TempBox->m_Dimensions.x / 2;
        double xPlus = m_TempBox->m_Center.x - m_TempBox->m_Dimensions.x / 2;
        double yMargin = m_TempBox->m_Dimensions.y * marginRatio;
        double yMinus = m_TempBox->m_Center.y - m_TempBox->m_Dimensions.y / 2;
        double yPlus = m_TempBox->m_Center.y + m_TempBox->m_Dimensions.y / 2;

        Jam3D::Vec4<double> xMinusRect(xMinus - xMargin, yMinus, xMargin * 2, m_TempBox->m_Dimensions.y);
        Jam3D::Vec4<double> xPlusRect(xPlus - xMargin, yMinus, xMargin * 2, m_TempBox->m_Dimensions.y);
        Jam3D::Vec4<double> yMinusRect(xPlus, yMinus - yMargin, m_TempBox->m_Dimensions.x, yMargin * 2);
        Jam3D::Vec4<double> yPlusRect(xPlus, yPlus - yMargin, m_TempBox->m_Dimensions.x, yMargin * 2);

        if (MathHelper::IsInRect(worldCoords.x, worldCoords.y, xMinusRect))
        {
            m_MouseLeftPressedTempBoxXMinusMargin = true;
        }

        else if (MathHelper::IsInRect(worldCoords.x, worldCoords.y, xPlusRect))
        {
            m_MouseLeftPressedTempBoxXPlusMargin = true;
        }

        if (MathHelper::IsInRect(worldCoords.x, worldCoords.y, yMinusRect))
        {
            m_MouseLeftPressedTempBoxYMinusMargin = true;
        }

        else if (MathHelper::IsInRect(worldCoords.x, worldCoords.y, yPlusRect))
        {
            m_MouseLeftPressedTempBoxYPlusMargin = true;
        }
    }

    else if (y && z)
    {
        double yMargin = m_TempBox->m_Dimensions.y * marginRatio;
        double yMinus = m_TempBox->m_Center.y - m_TempBox->m_Dimensions.y / 2;
        double yPlus = m_TempBox->m_Center.y + m_TempBox->m_Dimensions.y / 2;
        double zMargin = m_TempBox->m_Dimensions.z * marginRatio;
        double zMinus = m_TempBox->m_Center.z - m_TempBox->m_Dimensions.z / 2;
        double zPlus = m_TempBox->m_Center.z + m_TempBox->m_Dimensions.z / 2;

        Jam3D::Vec4<double> yMinusRect(yMinus - yMargin, zMinus, yMargin * 2, m_TempBox->m_Dimensions.z);
        Jam3D::Vec4<double> yPlusRect(yPlus - yMargin, zMinus, yMargin * 2, m_TempBox->m_Dimensions.z);
        Jam3D::Vec4<double> zMinusRect(yMinus, zMinus - zMargin, m_TempBox->m_Dimensions.y, zMargin * 2);
        Jam3D::Vec4<double> zPlusRect(yMinus, zPlus - zMargin, m_TempBox->m_Dimensions.y, zMargin * 2);

        if (MathHelper::IsInRect(worldCoords.y, worldCoords.z, yMinusRect))
        {
            m_MouseLeftPressedTempBoxYMinusMargin = true;
        }

        else if (MathHelper::IsInRect(worldCoords.y, worldCoords.z, yPlusRect))
        {
            m_MouseLeftPressedTempBoxYPlusMargin = true;
        }

        if (MathHelper::IsInRect(worldCoords.y, worldCoords.z, zMinusRect))
        {
            m_MouseLeftPressedTempBoxZMinusMargin = true;
        }

        else if (MathHelper::IsInRect(worldCoords.y, worldCoords.z, zPlusRect))
        {
            m_MouseLeftPressedTempBoxZPlusMargin = true;
        }
    }
}

void ObjectCreationView::CursorPosCallback(double xPos, double yPos)
{
    yPos = m_Window->m_Height - yPos;
    // Right Click
    if (m_MouseRightPressedUpperLeft)
        m_UpperLeftCamera->CursorPosCallback(xPos, yPos);

    // Left Click
    if (m_MouseLeftPressedUpperRight)
    {
        if (m_TempBox)
            UpdateTempBox(*m_UpperRightCamera, m_Viewports[m_UpperRightViewportIndex], xPos, yPos, true, false, true);
    }

    if (m_MouseLeftPressedLowerLeft)
    {
        if (m_TempBox)
            UpdateTempBox(*m_LowerLeftCamera, m_Viewports[m_LowerLeftViewportIndex], xPos, yPos, true, true, false);
    }

    if (m_MouseLeftPressedLowerRight)
    {
        if (m_TempBox)
            UpdateTempBox(*m_LowerRightCamera, m_Viewports[m_LowerRightViewportIndex], xPos, yPos, false, true, true);
    }

    if (m_MouseLeftPressedBetweenLeftRight)
    {
        ResizeViewports(xPos, true, false);
    }

    if (m_MouseLeftPressedBetweenUpperLower)
    {
        ResizeViewports(yPos, false, true);
    }

    // Upper-left: Normal
    else if (MathHelper::IsInRect(xPos, yPos, m_Viewports[m_UpperLeftViewportIndex].m_Corners))
        m_UpperLeftCamera->CursorPosCallback(xPos, yPos);
}

void ObjectCreationView::KeyCallback(int key, int scancode, int action, int mods)
{
    Jam3D::Vec2<double> cursorPos = m_Window->GetCursorPos();

    // Upper-left: Normal
    if (MathHelper::IsInRect(cursorPos.x, cursorPos.y, m_Viewports[m_UpperLeftViewportIndex].m_Corners))
        m_UpperLeftCamera->KeyCallback(key, scancode, action, mods);
}

void ObjectCreationView::MouseButtonCallback(int button, int action, int mods)
{
    Jam3D::Vec2<double> cursorPos = m_Window->GetCursorPos();

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        m_UpperLeftCamera->MouseButtonCallback(button, action, mods);
        m_MouseRightPressedUpperLeft = false;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        m_MouseLeftPressedUpperLeft = false;
        m_MouseLeftPressedUpperRight = false;
        m_MouseLeftPressedLowerLeft = false;
        m_MouseLeftPressedLowerRight = false;
        m_MouseLeftPressedBetweenLeftRight = false;
        m_MouseLeftPressedBetweenUpperLower = false;

        m_ViewportmiddleOriginal = m_ViewportMiddle;
        
        if (m_TempBox)
        {
            m_MouseLeftPressedTempBoxXMinusMargin = false;
            m_MouseLeftPressedTempBoxXPlusMargin = false;
            m_MouseLeftPressedTempBoxYMinusMargin = false;
            m_MouseLeftPressedTempBoxYPlusMargin = false;
            m_MouseLeftPressedTempBoxZMinusMargin = false;
            m_MouseLeftPressedTempBoxZPlusMargin = false;

            m_TempBoxCenterOriginal = m_TempBox->m_Center;
            m_TempBoxDimensionsOriginal = m_TempBox->m_Dimensions;
        }
    }

    // Upper-left: Normal
    if (MathHelper::IsInRect(cursorPos.x, cursorPos.y, m_Viewports[m_UpperLeftViewportIndex].m_Corners))
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            m_MouseRightPressedUpperLeft = true;
            m_MouseLeftPressedUpperLeft = true;
        }
        m_UpperLeftCamera->MouseButtonCallback(button, action, mods);
    }

    // Upper-Right: XZ
    else if (MathHelper::IsInRect(cursorPos.x, cursorPos.y, m_Viewports[m_UpperRightViewportIndex].m_Corners))
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glm::vec4 viewport = Jam3D::MathHelper::Vec4ToGlm(m_Viewports[m_UpperRightViewportIndex].m_Corners);
            glm::vec3 worldCoords = glm::unProject(glm::vec3(cursorPos.x, cursorPos.y, 1.0f), m_UpperRightCamera->m_ViewMatrix, m_UpperRightCamera->m_ProjectionMatrix, viewport);
            
            m_MouseLeftPressedUpperRight = true;
            m_MouseLeftPressLocation.x = worldCoords.x;
            m_MouseLeftPressLocation.z = worldCoords.z;

            if (m_TempBox && m_CurrentTool == ObjectCreationView::STRECH)
                IsInMargin(worldCoords, true, false, true);
        }
    }

    // Lower-Left: XY
    else if (MathHelper::IsInRect(cursorPos.x, cursorPos.y, m_Viewports[m_LowerLeftViewportIndex].m_Corners))
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glm::vec4 viewport = Jam3D::MathHelper::Vec4ToGlm(m_Viewports[m_LowerLeftViewportIndex].m_Corners);
            glm::vec3 worldCoords = glm::unProject(glm::vec3(cursorPos.x, cursorPos.y, 1.0f), m_LowerLeftCamera->m_ViewMatrix, m_LowerLeftCamera->m_ProjectionMatrix, viewport);
            
            m_MouseLeftPressedLowerLeft = true;
            m_MouseLeftPressLocation.x = worldCoords.x;
            m_MouseLeftPressLocation.y = worldCoords.y;

            if (m_TempBox && m_CurrentTool == ObjectCreationView::STRECH)
                IsInMargin(worldCoords, true, true, false);
        }
    }

    // Lower-Right: YZ
    else if (MathHelper::IsInRect(cursorPos.x, cursorPos.y, m_Viewports[m_LowerRightViewportIndex].m_Corners))
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glm::vec4 viewport = Jam3D::MathHelper::Vec4ToGlm(m_Viewports[m_LowerRightViewportIndex].m_Corners);
            glm::vec3 worldCoords = glm::unProject(glm::vec3(cursorPos.x, cursorPos.y, 1.0f), m_LowerRightCamera->m_ViewMatrix, m_LowerRightCamera->m_ProjectionMatrix, viewport);
            
            m_MouseLeftPressedLowerRight = true;
            m_MouseLeftPressLocation.z = worldCoords.z;
            m_MouseLeftPressLocation.y = worldCoords.y;

            if (m_TempBox && m_CurrentTool == ObjectCreationView::STRECH)
                IsInMargin(worldCoords, false, true, true);
        }
    }
    // Not in viewport -> must be between viewports.
    else 
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            m_MouseLeftPressLocation.x = cursorPos.x;
            m_MouseLeftPressLocation.y = cursorPos.y;

            // Between left and right
            if (cursorPos.x > m_Viewports[m_UpperLeftViewportIndex].m_Corners.x0 + m_Viewports[m_UpperLeftViewportIndex].m_Corners.x1
                && cursorPos.x < m_Viewports[m_UpperRightViewportIndex].m_Corners.x0)
            {
                m_MouseLeftPressedBetweenLeftRight = true;
            }
            // Between upper and lower
            if (cursorPos.y < m_Viewports[m_UpperLeftViewportIndex].m_Corners.y0
                && cursorPos.y > m_Viewports[m_LowerLeftViewportIndex].m_Corners.y0 + m_Viewports[m_LowerLeftViewportIndex].m_Corners.y1)
            {
                m_MouseLeftPressedBetweenUpperLower = true;
            }
        }
    }
}

void ObjectCreationView::ScrollCallback(double yOffset)
{
    Jam3D::Vec2<double> cursorPos = m_Window->GetCursorPos();

    // Upper-left: Normal
    if (MathHelper::IsInRect(cursorPos.x, cursorPos.y, m_Viewports[m_UpperLeftViewportIndex].m_Corners))
        m_UpperLeftCamera->ScrollCallback(yOffset);
}

}