#include "engine/utils/camera_controller.hpp"
#include "engine/panels/base_view.hpp"
#include "opengl/camera.hpp"
#include "tools/utils/service_locator.hpp"
#include "windows/inputs/input_manager.hpp"
#include "windows/sdl2.hpp"

#include <imgui/imgui.h>

namespace Inputs = Engine::Windows::Inputs;
namespace Panels = Engine::Panels;
namespace Utils = Engine::Utils;

Utils::CameraController::CameraController
(
    Panels::BaseView* view,
    std::shared_ptr<OpenGL::Camera> camera,
    Math::FVector3& position,
    Math::FQuaternion& rotation,
    bool enableFocusInputs
) : m_view { view },
    m_camera { camera },
    m_cameraPosition(position),
    m_cameraRotation(rotation)
{
    m_camera->SetFov(60.0f);
}

void Utils::CameraController::HandleInputs(float dt)
{
    if (m_view->IsHovered())
    {
        handleCameraFPSKeyboard(dt);
    }
}

void Utils::CameraController::CameraController::SetSpeed(float speed)
{
    m_cameraMoveSpeed = speed;
}

float Utils::CameraController::CameraController::GetSpeed() const
{
    return m_cameraMoveSpeed;
}

void Utils::CameraController::SetPosition(const Math::FVector3 position)
{
    m_cameraPosition = position;
}

void Utils::CameraController::SetRotation(const Math::FQuaternion rotation)
{
    m_cameraRotation = rotation;
}

const Engine::Math::FVector3& Utils::CameraController::GetPosition() const
{
    return m_cameraPosition;
}

const Engine::Math::FQuaternion& Utils::CameraController::GetRotation() const
{
    return m_cameraRotation;
}

void Utils::CameraController::handleCameraFPSKeyboard(float dt)
{
    std::shared_ptr<Inputs::InputManager> inputManager = Tools::Utils::ServiceLocator::Get<Inputs::InputManager>();
    Math::FVector3 targetSpeed = Math::FVector3(0.0f, 0.0f, 0.0f);

    bool run = inputManager->IsKeyPressed(Inputs::EKey::LShift);
    float velocity = m_cameraMoveSpeed * dt * (run ? 2.0f : 1.0f);

    if (inputManager->IsKeyPressed(Inputs::EKey::W))
        targetSpeed += m_cameraRotation * Math::FVector3::GetForward() * velocity;
    if (inputManager->IsKeyPressed(Inputs::EKey::S))
        targetSpeed += m_cameraRotation * Math::FVector3::GetForward() * -velocity;
    if (inputManager->IsKeyPressed(Inputs::EKey::A))
        targetSpeed += m_cameraRotation * Math::FVector3::GetRight() * velocity;
    if (inputManager->IsKeyPressed(Inputs::EKey::D))
        targetSpeed += m_cameraRotation * Math::FVector3::GetRight() * -velocity;
    if (inputManager->IsKeyPressed(Inputs::EKey::E))
        targetSpeed += { 0.0f, velocity, 0.0f };
    if (inputManager->IsKeyPressed(Inputs::EKey::Q))
        targetSpeed += { 0.0f, -velocity, 0.0f };

    m_currentMovementSpeed = Math::FVector3::Lerp(m_currentMovementSpeed, targetSpeed, 10.0f * dt);
    m_cameraPosition += m_currentMovementSpeed;
}
