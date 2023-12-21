#pragma once
#ifndef ENGINE_OPENGL_CAMERA_HPP_
#define ENGINE_OPENGL_CAMERA_HPP_

#include "math/quaternion.hpp"
#include "settings/projection_mode.hpp"

namespace Engine
{
namespace OpenGL
{
    class Camera
    {
    public:
        Camera()  = default;
        ~Camera() = default;

        Camera(const Camera& other)             = delete;
        Camera(Camera&& other)                  = delete;
        Camera& operator=(const Camera& other)  = delete;
        Camera& operator=(const Camera&& other) = delete;

        void CacheMatrices(uint16_t windowWidth, uint16_t windowHeight, const Math::FVector3& position, const Math::FQuaternion& rotation);
        void CacheProjectionMatrix(uint16_t windowWidth, uint16_t windowHeight);
        void CacheViewMatrix(const Math::FVector3& position, const Math::FQuaternion& rotation);

        float GetFov() const;
        float GetSize() const;
        float GetNear() const;
        float GetFar() const;

        const Math::FMatrix4& GetProjectionMatrix() const;
        const Math::FMatrix4& GetViewMatrix() const;
        Settings::EProjectionMode GetProjectionMode() const;

        void SetFov(float value);
        void SetSize(float value);
        void SetNear(float value);
        void SetFar(float value);

        void SetProjectionMode(Settings::EProjectionMode projectionMode);

    private:
        Math::FMatrix4 calculateProjectionMatrix(uint16_t windowWidth, uint16_t windowHeight) const;
        Math::FMatrix4 calculateViewMatrix(const Math::FVector3& position, const Math::FQuaternion& rotation) const;

        Math::FMatrix4 m_viewMatrix;
        Math::FMatrix4 m_projectionMatrix;
        Settings::EProjectionMode m_projectionMode = Settings::EProjectionMode::Perspective;

        float m_fov  = 45.0f;
        float m_size = 5.0f;
        float m_near = 0.1f;
        float m_far  = 100.0f;
    };
}
}

#endif // ENGINE_OPENGL_CAMERA_HPP_