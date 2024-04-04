#pragma once
#ifndef ENGINE_UTILS_CAMERA_CONTROLLER_HPP_
#define ENGINE_UTILS_CAMERA_CONTROLLER_HPP_

#include <memory>

#include "math/quaternion.hpp"
#include "tools/eventing/event.hpp"

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
        void handleCameraFPSMouse(float dt);

        void onMouseButtonPressed(int button);
        void onMouseButtonReleased(int button);

        Panels::BaseView* m_view = nullptr;
        std::shared_ptr<OpenGL::Camera> m_camera = nullptr;
        Math::FVector3& m_cameraPosition;
        Math::FQuaternion& m_cameraRotation;

        Math::FVector3 m_currentMovementSpeed = Math::FVector3(0.0f, 0.0f, 0.0f);
        Math::FVector3 m_cameraAngle = Math::FVector3(0.0f, 0.0f, 0.0f);

        //bool m_firstStep = true;
        Tools::Eventing::ListenerID m_mouseButtonPressedListener;
        Tools::Eventing::ListenerID m_mouseButtonReleasedListener;

        float m_cameraMoveSpeed = 15.0f;
        float m_mouseSensitivity = 5.0f;
    };
}

#endif // ENGINE_UTILS_CAMERA_CONTROLLER_HPP_
