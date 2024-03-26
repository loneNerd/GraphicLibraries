#pragma once
#ifndef ENGINE_UTILS_CAMERA_CONTROLLER_HPP_
#define ENGINE_UTILS_CAMERA_CONTROLLER_HPP_

#include <memory>

#include "math/quaternion.hpp"

namespace Engine::OpenGL { class Camera; }
namespace Engine::Panels { class BaseView; }

namespace Engine::Utils
{
    class CameraController
    {
    public:
        CameraController
        (
            Panels::BaseView* view,
            std::shared_ptr<OpenGL::Camera> camera,
            Math::FVector3& position,
            Math::FQuaternion& rotation,
            bool enableFocusInputs = false
        );

        CameraController(const CameraController& other)             = delete;
        CameraController(CameraController&& other)                  = delete;
        CameraController& operator=(const CameraController& other)  = delete;
        CameraController& operator=(const CameraController&& other) = delete;

        void HandleInputs(float dt);

        float GetSpeed() const;

        void SetSpeed(float speed);
        void SetPosition(const Math::FVector3 position);
        void SetRotation(const Math::FQuaternion rotation);

        const Math::FVector3& GetPosition() const;
        const Math::FQuaternion& GetRotation() const;

    private:
        void handleCameraFPSKeyboard(float dt);

        Panels::BaseView* m_view = nullptr;
        std::shared_ptr<OpenGL::Camera> m_camera = nullptr;
        Math::FVector3& m_cameraPosition;
        Math::FQuaternion& m_cameraRotation;

        //bool m_leftMousePressed = false;
        //bool m_middleMousePressed = false;
        //bool m_rightMousePressed = false;

        //Math::FVector3 m_targetSpeed = Math::FVector3(0.0f, 0.0f, 0.0f);
        Math::FVector3 m_currentMovementSpeed = Math::FVector3(0.0f, 0.0f, 0.0f);

        //bool m_firstMouse = true;
        //double m_lastMousePosX = 0.0;
        //double m_lastMousePosY = 0.0;
        //Math::FVector3 m_ypr = Math::FVector3(0.0f, 0.0f, 0.0f);
        //float m_mouseSensitivity = 0.12f;
        //float m_cameraDragSpeed = 0.03f;
        //float m_cameraOrbitSpeed = 0.5f;
        float m_cameraMoveSpeed = 15.0f;
    };
}

#endif // ENGINE_UTILS_CAMERA_CONTROLLER_HPP_
