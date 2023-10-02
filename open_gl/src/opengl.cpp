#include "opengl.hpp"

#include <iostream>
#include <sstream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "utils.hpp"
#include "resource_manager.hpp"
#include "widgets/fps_counter.hpp"
#include "windows/glfw.hpp"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Widgets;
using namespace GraphicLibraries::Windows;

OpenGL::OpenGL()
    : m_window { nullptr },
      m_fpsCounter { nullptr }
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

    m_triangle.init();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
    ImGui_ImplOpenGL3_Init(m_glslVersion);

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

    if (m_fpsCounter)
    {
        delete m_fpsCounter;
        m_fpsCounter = nullptr;
    }

    m_triangle.release();

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
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Elements", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SeparatorText("Triangle");
        m_triangle.update(dt);
    }

    ImGui::End();

    m_triangle.draw();
    m_fpsCounter->draw();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window->getWindow());
    glfwPollEvents();

    ResourceManager::collectGarbage();
}
