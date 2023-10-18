#include "glfw.hpp"

#include <iostream>
#include <stdexcept>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "interfaces/level.hpp"
#include "objects/levels/simple_2d.hpp"
//#include "objects/renderer.hpp"
#include "resource_manager.hpp"
#include "widgets/fps_counter.hpp"
#include "window_observers.hpp"

using namespace GraphicLibraries::OpenGL::Interfaces;
using namespace GraphicLibraries::OpenGL::Objects::Levels;
using namespace GraphicLibraries::OpenGL::Widgets;
using namespace GraphicLibraries::OpenGL::Windows;

GLFWWindow::GLFWWindow()
{
    m_isCursorDisable = false;
    m_isClosed = true;
    m_isInit = false;
}

GLFWWindow::~GLFWWindow()
{
    if (m_isInit)
        release();
}

void GLFWWindow::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "OpenGL", NULL, NULL);
    if (!m_window)
        throw std::runtime_error("GLFW: Failed to create GLFW window");

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);
    glfwSetWindowUserPointer(m_window, this);

    glfwSetCursorPosCallback(m_window,   setCursorPosCallback);
    glfwSetKeyCallback(m_window,         setKeyCallback);
    glfwSetMouseButtonCallback(m_window, setMouseButtonCallback);

    //m_renderer = std::make_unique<Renderer>();

    //if (!m_renderer)
    //    throw std::runtime_error("GLFW: Can't create opengl renderer");

    //m_renderer->init(this);

    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();

    if (glewStatus != 0)
        throw std::runtime_error(std::string("OPENGL: ").append(reinterpret_cast<const char*>(glewGetErrorString(glewStatus))));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(m_glslVersion);

    m_fpsCounter = std::make_unique<FpsCounter>();

    if (!m_fpsCounter)
        throw std::runtime_error("OPENGL: Can't initialize FPS Counter widget");

    std::shared_ptr<ILevel> simple2D = std::make_shared<Simple2D>();

    if (!simple2D)
        throw std::runtime_error("RENDERER: Can't create simple 2d level");

    simple2D->init(this);
    m_currentLevel = simple2D;

    m_levels["simple_2d"] = simple2D;

    m_isInit = true;
    m_isClosed = false;
}

void GLFWWindow::release()
{
    m_inputObserversList.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (auto& level : m_levels)
    {
        if (level.second)
        {
            level.second->release();
            level.second = nullptr;
        }
    }

    m_levels.clear();

    if (m_fpsCounter)
        m_fpsCounter = nullptr;

    ResourceManager::clear();

    glfwTerminate();
    m_isInit = false;
}

void GLFWWindow::handleEvent()
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    if (glfwWindowShouldClose(m_window))
        m_isClosed = true;
}

void GLFWWindow::newFrame(float dt)
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Elements", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SeparatorText("Triangle");
        m_currentLevel->update(dt);
    }

    ImGui::End();

    m_currentLevel->draw();
    m_fpsCounter->draw();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    ResourceManager::collectGarbage();
}

bool GLFWWindow::getKeyStatus(int key)
{
    if (key > GLFW_KEY_UNKNOWN && key <= GLFW_KEY_LAST)
        return glfwGetKey(m_window, key);

    return false;
}

void GLFWWindow::setCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    GLFWWindow* ptr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

    static bool firstMove = false;
    static float lastX = 0.0f;
    static float lastY = 0.0f;

    if (firstMove)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMove = false;
    }

    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(ypos - lastY);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    for (IInputObserver* observer : ptr->m_inputObserversList)
    {
        if (observer) observer->mouseMovementCallback(xoffset, yoffset);
    }
}

void GLFWWindow::setKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLFWWindow* ptr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

    for (IInputObserver* observer : ptr->m_inputObserversList)
    {
        if (observer)
        {
            if (action == GLFW_PRESS)
                observer->keyPressedCallback(key);
            else
                observer->keyReleaseCallback(key);
        }
    }
}

void GLFWWindow::setMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    GLFWWindow* ptr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

    for (IInputObserver* observer : ptr->m_inputObserversList)
    {
        if (observer)
        {
            if (action == GLFW_PRESS)
                observer->mouseButtonPressedCallback(button);
            else
                observer->mouseButtonReleaseCallback(button);
        }
    }
}

void GLFWWindow::setTitle(const char* title)
{
    if (m_window)
        glfwSetWindowTitle(m_window, title);
}

void GLFWWindow::showCursor()
{
    if (!m_isCursorDisable)
        return;

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_isCursorDisable = false;
}

void GLFWWindow::hideCursor()
{
    if (m_isCursorDisable)
        return;

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_isCursorDisable = true;
}

void GLFWWindow::toggleCursor()
{
    m_isCursorDisable ?
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL) :
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
