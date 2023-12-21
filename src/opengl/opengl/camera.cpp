#include "camera.hpp"

namespace Math = Engine::Math;
namespace OpenGL = Engine::OpenGL;
namespace Settings = Engine::OpenGL::Settings;

void OpenGL::Camera::CacheMatrices(uint16_t windowWidth, uint16_t windowHeight, const Math::FVector3& position, const Math::FQuaternion& rotation)
{
    CacheProjectionMatrix(windowWidth, windowHeight);
    CacheViewMatrix(position, rotation);
}

void OpenGL::Camera::CacheProjectionMatrix(uint16_t windowWidth, uint16_t windowHeight)
{
    m_projectionMatrix = calculateProjectionMatrix(windowWidth, windowHeight);
}

void OpenGL::Camera::CacheViewMatrix(const Math::FVector3& position, const Math::FQuaternion& rotation)
{
    m_viewMatrix = calculateViewMatrix(position, rotation);
}

float OpenGL::Camera::GetFov() const
{
    return m_fov;
}

float OpenGL::Camera::GetSize() const
{
    return m_size;
}

float OpenGL::Camera::GetNear() const
{
    return m_near;
}

float OpenGL::Camera::GetFar() const
{
    return m_far;
}

const Math::FMatrix4& OpenGL::Camera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

const Math::FMatrix4& OpenGL::Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

Settings::EProjectionMode OpenGL::Camera::GetProjectionMode() const
{
    return m_projectionMode;
}

void OpenGL::Camera::SetFov(float value)
{
    m_fov = value;
}

void OpenGL::Camera::SetSize(float value)
{
    m_size = value;
}

void OpenGL::Camera::SetNear(float value)
{
    m_near = value;
}

void OpenGL::Camera::SetFar(float value)
{
    m_far = value;
}

void OpenGL::Camera::SetProjectionMode(Settings::EProjectionMode projectionMode)
{
    m_projectionMode = projectionMode;
}

Math::FMatrix4 OpenGL::Camera::calculateProjectionMatrix(uint16_t windowWidth, uint16_t windowHeight) const
{
    const auto ratio = windowWidth / static_cast<float>(windowHeight);

    switch (m_projectionMode)
    {
    case Settings::EProjectionMode::Orthographic:
        return Math::FMatrix4::CreateOrthographic(m_size, ratio, m_near, m_far);

    case Settings::EProjectionMode::Perspective:
        return Math::FMatrix4::CreatePerspective(m_fov, ratio, m_near, m_far);

    default:
        return Math::FMatrix4::Identity;
    }
}

Math::FMatrix4 OpenGL::Camera::calculateViewMatrix(const Math::FVector3& position, const Math::FQuaternion& rotation) const
{
    const Math::FVector3& up = rotation * Math::FVector3::Up;
    const Math::FVector3& forward = rotation * Math::FVector3::Forward;

    return Math::FMatrix4::CreateView
    (
        position.X, position.Y, position.Z,
        position.X + forward.X, position.Y + forward.Y, position.Z + forward.Z,
        up.X, up.Y, up.Z
    );
}
