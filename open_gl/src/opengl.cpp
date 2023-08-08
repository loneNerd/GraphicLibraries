#include "opengl.hpp"

#include <iostream>
#include <sstream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include "utils.hpp"
#include "widgets/fps_counter.hpp"
#include "windows/sdl2.hpp"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Widgets;
using namespace GraphicLibraries::Windows;

OpenGL::OpenGL()
    : m_window { nullptr },
      m_fpsCounter { nullptr },
      m_context { NULL }
{
    m_isInit = false;
}

OpenGL::~OpenGL()
{
    std::cout << "OpenGL Destructor" << std::endl;

    if (m_isInit)
        release();
}

void OpenGL::init()
{
    std::cout << "Initializing OpenGL" << std::endl;

    m_window = new SDL2Window;

    if (!m_window)
        throw std::runtime_error("OPENGL: Can't create sdl2 window");

    m_window->initForOpenGL();
    m_window->setTitle("OpenGL");

    m_context = SDL_GL_CreateContext(m_window->getWindow());

    if (m_context == NULL)
        throw std::runtime_error("OPENGL: Can't create SDL context");

    SDL_GL_MakeCurrent(m_window->getWindow(), m_context);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

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

    ImGui_ImplSDL2_InitForOpenGL(m_window->getWindow(), m_context);
    ImGui_ImplOpenGL3_Init(m_glslVersion);

    m_fpsCounter = new FpsCounter;

    if (!m_fpsCounter)
        throw std::runtime_error("OPENGL: Can't initialize FPS Counter widget");

    m_isInit = true;
}

void OpenGL::release()
{
    std::cout << "Releasing OpenGL" << std::endl;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_fpsCounter)
    {
        delete m_fpsCounter;
        m_fpsCounter = nullptr;
    }

    m_triangle.release();

    SDL_GL_DeleteContext(m_context);

    if (m_window)
    {
        m_window->release();
        delete m_window;
        m_window = nullptr;
    }

    m_isInit = false;
}

void OpenGL::newFrame(float dt)
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplSDL2_ProcessEvent(&m_window->getEvent());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
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

    SDL_GL_SwapWindow(m_window->getWindow());
}
