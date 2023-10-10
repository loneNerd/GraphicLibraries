#include "camera.hpp"

#include <iostream>

#include <glfw/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "windows/glfw.hpp"
#include "windows/window_observers.hpp"

using namespace GraphicLibraries::OpenGL::Objects;

Camera::Camera(Windows::GLFWWindow* window)
{
    if (window)
    {
        m_window = window;
        m_window->attachInputObserver(this);
    }
}

Camera::~Camera()
{
    if (m_window)
    {
        m_window->detachInputObserver(this);
    }
}

void Camera::update(float dt)
{
    if (!m_window)
        return;

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

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt( m_position, m_position + m_front, m_up );
}

//void Camera::setOrthoProjection( float left, float right, float bottom, float top, float zNear, float zFar )
//{
//    m_projection = glm::ortho( left, right, bottom, top, zNear, zFar );
//}

//void Camera::setPerspectiveProjection( float fov, float aspectRatio, float zNear, float zFar )
//{
//    m_projection = glm::perspective( fov, aspectRatio, zNear, zFar );
//}

void Camera::setProjection(float fov, float aspectRatio, float zNear, float zFar)
{
    setProjection(glm::perspective(fov, aspectRatio, zNear, zFar));
}

void Camera::updateVectors()
{
    // calculate the new Front vector
    glm::vec3 front = glm::vec3( 0.0f );
    front.x = cos( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
    front.y = sin( glm::radians( m_pitch ) );
    front.z = sin( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
    m_front = glm::normalize( front );
    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_right = glm::normalize( glm::cross( m_front, m_worldUp ) );
    m_up = glm::normalize( glm::cross( m_right, m_front ) );
}

void Camera::mouseMovementCallback(float xpos, float ypos)
{
    if (!m_window || !m_window->isRightMouseBtn())
        return;

    setYaw(m_yaw += (xpos / m_sensitivity));
    setPitch(m_pitch -= (ypos / m_sensitivity));
}

void Camera::mouseButtonPressedCallback(int button)
{
    if (!m_window)
        return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        m_window->hideCursor();
}

void Camera::mouseButtonReleaseCallback(int button)
{
    if (!m_window)
        return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        m_window->showCursor();
}
