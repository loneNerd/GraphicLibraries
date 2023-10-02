#include "glfw.hpp"

#include <iostream>
#include <stdexcept>

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

    glfwSetWindowUserPointer(m_window, this);

    //glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
    //{
    //    GLFWWindow* ptr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
    //    ptr->m_isClosed = true;
    //});

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

void GLFWWindow::setTitle(const char* title)
{
    if (m_window)
        glfwSetWindowTitle(m_window, title);
}
