#include "moveble_camera.hpp"

#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "windows/glfw.hpp"
#include "windows/window_observers.hpp"

using namespace GraphicLibraries::OpenGL::Objects;

MovebleCamera::MovebleCamera(Windows::GLFWWindow* window)
{
    if (!window)
        throw std::runtime_error("MOVEBLE CAMERA: window pointer is null");

    m_window = window;
    m_window->attachInputObserver(this);
}

MovebleCamera::~MovebleCamera()
{
    m_window->detachInputObserver(this);
}

void MovebleCamera::update(float dt)
{
    float velocity = m_speed * dt;

    if (m_window->getKeyStatus(GLFW_KEY_W))
        movePosition(m_front * velocity);
    if (m_window->getKeyStatus(GLFW_KEY_S))
        movePosition(m_front * -velocity);
    if (m_window->getKeyStatus(GLFW_KEY_A))
        movePosition(m_right * -velocity);
    if (m_window->getKeyStatus(GLFW_KEY_D))
        movePosition(m_right * velocity);
    if (m_window->getKeyStatus(GLFW_KEY_E) || m_window->getKeyStatus(GLFW_KEY_SPACE))
        movePosition(m_up * velocity);
    if (m_window->getKeyStatus(GLFW_KEY_Q) || m_window->getKeyStatus(GLFW_KEY_C) || m_window->getKeyStatus(GLFW_KEY_LEFT_CONTROL))
        movePosition(m_up * -velocity);
}

void MovebleCamera::setProjection(float fov, float aspectRatio, float zNear, float zFar)
{
    ICamera::setProjection(glm::perspective(fov, aspectRatio, zNear, zFar));
}

void MovebleCamera::mouseMovementCallback(float xpos, float ypos)
{
    if (!m_window->isRightMouseBtn())
        return;

    setYaw(m_yaw += (xpos / m_sensitivity));
    setPitch(m_pitch -= (ypos / m_sensitivity));
}

void MovebleCamera::mouseButtonPressedCallback(int button)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        m_window->hideCursor();
}

void MovebleCamera::mouseButtonReleaseCallback(int button)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        m_window->showCursor();
}
