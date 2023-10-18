#include "simple_2d.hpp"

#include "interfaces/camera.hpp"
#include "objects/2d_models/triangle.hpp"
#include "objects/moveble_camera.hpp"
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

    std::shared_ptr<MovebleCamera> camera = std::make_shared<MovebleCamera>(m_window);

    if (!camera)
        throw std::runtime_error("OPENGL: Can't initialize camera");

    camera->setYaw(-90.0f);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    camera->setWorldUp(glm::vec3(0.0f, 1.0f, 0.0f));
    camera->setProjection(45.0f,
                            static_cast<float>(m_window->getWidth()) / static_cast<float>(m_window->getHeight()),
                            0.1f, 100.0f);
    camera->updateVectors();

    m_camerasList.emplace_back(camera);
    m_currentCamera = *(m_camerasList.begin());

    m_isInit = true;
}

void Simple2D::release()
{
    for (std::shared_ptr<ICamera>& camera : m_camerasList)
        camera = nullptr;

    m_camerasList.clear();

    if (m_currentCamera)
        m_currentCamera = nullptr;

    if (m_triangle)
    {
        m_triangle->release();
        m_triangle = nullptr;
    }

    ResourceManager::clear();

    m_isInit = false;
}

void Simple2D::update(float dt)
{
    m_triangle->update(dt);
}

void Simple2D::draw()
{
    m_triangle->draw(m_currentCamera);
}
