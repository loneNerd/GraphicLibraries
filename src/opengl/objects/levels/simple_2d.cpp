#include "simple_2d.hpp"

#include <algorithm>

#include <imgui/imgui.h>

#include "interfaces/camera.hpp"
#include "objects/2d_models/triangle.hpp"
#include "objects/movable_camera.hpp"
#include "resource_manager.hpp"
#include "windows/glfw.hpp"

using namespace GraphicLibraries::OpenGL::Interfaces;
using namespace GraphicLibraries::OpenGL::Objects;
using namespace GraphicLibraries::OpenGL::Objects::Levels;
using namespace GraphicLibraries::OpenGL::Objects::Models2D;
using namespace GraphicLibraries::OpenGL::Windows;

Simple2D::Simple2D()
{
    m_isInit = false;
}

Simple2D::~Simple2D()
{
    if (m_isInit)
        release();
}

void Simple2D::init(Windows::GLFWWindow* window)
{
    if (!window)
        throw std::runtime_error("SIMPLE 2D: Window is empty");

    m_window = window;

    m_triangle = std::make_unique<Triangle>();

    if (!m_triangle)
        throw std::runtime_error("SIMPLE 2D: Can't create triangle");

    m_triangle->init();

    std::shared_ptr<MovableCamera> camera = std::make_shared<MovableCamera>(m_window);

    if (!camera)
        throw std::runtime_error("OPENGL: Can't initialize camera");

    camera->setYaw(-90.0f);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    camera->setWorldUp(glm::vec3(0.0f, 1.0f, 0.0f));
    camera->setProjection(45.0f,
                          static_cast<float>(m_window->getWidth()) / static_cast<float>(m_window->getHeight()),
                          0.1f, 100.0f);
    camera->updateVectors();

    m_currentCamera = "Movable Camera";
    m_cameras[m_currentCamera] = camera;

    m_isInit = true;
}

void Simple2D::release()
{
    for (auto& camera : m_cameras)
    {
        if (camera.second)
            camera.second = nullptr;
    }

    m_cameras.clear();

    if (m_triangle)
    {
        m_triangle->release();
        m_triangle = nullptr;
    }

    ResourceManager::clear();

    m_isInit = false;
}

void Simple2D::updateUI()
{
    ImGui::NewLine();

    int counter = 0;
    static size_t cameraIdx = 0;
    std::vector<const char*> cameraValues;
    std::transform(m_cameras.begin(), m_cameras.end(), std::back_inserter(cameraValues),
        [&](const std::map<std::string, std::shared_ptr<Interfaces::ICamera>>::value_type& val)
        {
            if (val.first == m_currentCamera)
                cameraIdx = counter;

            ++counter;
            return val.first.c_str();
        });

    if (ImGui::BeginCombo("Cameras", cameraValues[cameraIdx], ImGuiComboFlags_PopupAlignLeft))
    {
        for (int n = 0; n < cameraValues.size(); ++n)
        {
            if (ImGui::Selectable(cameraValues[n], cameraIdx == n))
                cameraIdx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (cameraIdx == n)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    m_currentCamera = cameraValues[cameraIdx];

    ImGui::NewLine();

    if (ImGui::CollapsingHeader(m_currentCamera.c_str()))
        m_cameras[m_currentCamera]->updateUI();

    if (ImGui::CollapsingHeader("Triangle"))
        m_triangle->updateUI();
}

void Simple2D::update(float dt)
{
    m_cameras[m_currentCamera]->update(dt);
    m_triangle->update(dt);
}

void Simple2D::draw()
{
    m_triangle->draw(m_cameras[m_currentCamera]);
}
