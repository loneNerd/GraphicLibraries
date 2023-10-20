#include "movable_camera.hpp"

#include <iostream>
#include <stdexcept>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>

#include "windows/glfw.hpp"
#include "windows/window_observers.hpp"

using namespace GraphicLibraries::OpenGL::Objects;

MovableCamera::MovableCamera(Windows::GLFWWindow* window)
{
    if (!window)
        throw std::runtime_error("MOVEBLE CAMERA: window pointer is null");

    m_window = window;
    m_window->attachInputObserver(this);
}

MovableCamera::~MovableCamera()
{
    m_window->detachInputObserver(this);
}

void MovableCamera::updateUI()
{
    ImGui::NewLine();

    ImGui::InputFloat("Yaw", &m_yaw, 0.1f, m_yaw, "%.1f");
    ImGui::InputFloat("Pitch", &m_pitch, 0.1f, m_pitch, "%.1f");

    glm::vec3 camPos = m_position;
    float camPosArr[4] = { camPos.x, camPos.y, camPos.z, 0.0f };
    if (ImGui::InputFloat3("Position##Camera", camPosArr))
        m_position = glm::vec3(camPosArr[0], camPosArr[1], camPosArr[2]);

    glm::vec3 camWorldUp = m_worldUp;
    float camWorldUpArr[4] = { camWorldUp.x, camWorldUp.y, camWorldUp.z, 0.0f };
    if (ImGui::InputFloat3("World Up", camWorldUpArr))
        m_worldUp = glm::vec3(camWorldUpArr[0], camWorldUpArr[1], camWorldUpArr[2]);

    ImGui::InputFloat("Speed", &m_speed, 0.1f, m_speed, "%.1f");
    ImGui::InputFloat("Sensitivity", &m_sensitivity, 0.1f, m_sensitivity, "%.1f");

    ImGui::Text("Front X: %.1f; Y: %.1f; Z: %.1f;", m_front.x, m_front.y, m_front.z);
    ImGui::Text("Up X: %.1f; Y: %.1f; Z: %.1f;", m_up.x, m_up.y, m_up.z);
    ImGui::Text("Right X: %.1f; Y: %.1f; Z: %.1f;", m_right.x, m_right.y, m_right.z);
    ImGui::Text("Projection FOV: %.1f; Aspect Ratio: %.1f; Z Near: %.1f; Z Far: %.1f;", m_fov, m_aspectRatio, m_zNear, m_zFar);

    ImGui::NewLine();
}

void MovableCamera::update(float dt)
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

void MovableCamera::setProjection(float fov, float aspectRatio, float zNear, float zFar)
{
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_zNear = zNear;
    m_zFar = zFar;

    ICamera::setProjection(glm::perspective(fov, aspectRatio, zNear, zFar));
}

void MovableCamera::mouseMovementCallback(float xpos, float ypos)
{
    if (!m_window->isRightMouseBtn())
        return;

    setYaw(m_yaw += (xpos / m_sensitivity));
    setPitch(m_pitch -= (ypos / m_sensitivity));
}

void MovableCamera::mouseButtonPressedCallback(int button)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        m_window->hideCursor();
}

void MovableCamera::mouseButtonReleaseCallback(int button)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        m_window->showCursor();
}
