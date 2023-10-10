#include "opengl.hpp"

#include <iostream>
#include <sstream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "utils.hpp"
#include "resource_manager.hpp"
#include "widgets/fps_counter.hpp"
#include "windows/glfw.hpp"

using namespace GraphicLibraries::OpenGL::Objects;
using namespace GraphicLibraries::OpenGL::Widgets;
using namespace GraphicLibraries::OpenGL::Windows;

OpenGL::OpenGL()
    : m_window     { nullptr },
      m_fpsCounter { nullptr },
      m_camera     { nullptr }
{
    m_isInit = false;
}

OpenGL::~OpenGL()
{
    if (m_isInit)
        release();
}

void OpenGL::init()
{
    m_window = new GLFWWindow;

    if (!m_window)
        throw std::runtime_error("OPENGL: Can't create sdl2 window");

    m_window->initForOpenGL();
    m_window->setTitle("OpenGL");

    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();

    if (glewStatus != 0)
        throw std::runtime_error(std::string("OPENGL: ").append(reinterpret_cast<const char*>(glewGetErrorString(glewStatus))));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    //m_triangle.init();
    m_cube.init();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
    ImGui_ImplOpenGL3_Init(m_glslVersion);

    //m_camera = new Camera(m_window);
    m_camera = std::make_shared<Camera>(m_window);

    if (!m_camera)
        throw std::runtime_error("OPENGL: Can't initialize camera");

    m_camera->setYaw(-90.0f);
    m_camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    m_camera->setWorldUp(glm::vec3(0.0f, 1.0f, 0.0f));
    m_camera->setProjection(45.0f,
                            static_cast<float>(m_window->getWidth()) / static_cast<float>(m_window->getHeight()),
                            0.1f, 100.0f);
    m_camera->updateVectors();

    m_fpsCounter = new FpsCounter;

    if (!m_fpsCounter)
        throw std::runtime_error("OPENGL: Can't initialize FPS Counter widget");

    m_isInit = true;
}

void OpenGL::release()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_camera)
    {
        //delete m_camera;
        m_camera = nullptr;
    }

    if (m_fpsCounter)
    {
        delete m_fpsCounter;
        m_fpsCounter = nullptr;
    }

    //m_triangle.release();
    m_cube.release();

    if (m_window)
    {
        m_window->release();
        delete m_window;
        m_window = nullptr;
    }

    ResourceManager::clear();

    m_isInit = false;
}

void OpenGL::newFrame(float dt)
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Elements", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SeparatorText("Triangle");
        //m_triangle.update(dt);
        m_camera->update(dt);
        m_cube.update(dt);
    }

    ImGui::End();

    //m_triangle.draw(m_camera);
    m_cube.draw(m_camera);
    m_fpsCounter->draw();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window->getWindow());
    glfwPollEvents();

    ResourceManager::collectGarbage();
}
