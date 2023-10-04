#include "glfw.hpp"

#include <iostream>
#include <stdexcept>

#include "window_observers.hpp"

using namespace GraphicLibraries::Windows;

GLFWWindow::GLFWWindow()
{
    m_isClosed = true;
    m_isInit = false;
}

GLFWWindow::~GLFWWindow()
{
    if (m_isInit)
        release();
}

void GLFWWindow::initForOpenGL()
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

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
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
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
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
    });

    m_isInit = true;
    m_isClosed = false;
}

void GLFWWindow::release()
{
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

bool GLFWWindow::getKeyStatus(int key)
{
    if (key > GLFW_KEY_UNKNOWN && key <= GLFW_KEY_LAST)
        return glfwGetKey(m_window, key);

    return false;
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
