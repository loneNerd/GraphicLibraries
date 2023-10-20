#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_OBJECTS_CAMERA_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_OBJECTS_CAMERA_HPP_

#include <iostream>
#include <memory>

#include "interfaces/camera.hpp"
#include "windows/window_observers.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{

namespace Windows { class GLFWWindow; }

namespace Objects
{
    class MovableCamera : public Interfaces::ICamera, public Windows::IInputObserver
    {
    public:
        MovableCamera(Windows::GLFWWindow* window);
        virtual ~MovableCamera();

        MovableCamera(const MovableCamera& other)            = delete;
        MovableCamera(MovableCamera&& other)                 = delete;
        MovableCamera& operator=(const MovableCamera& other) = delete;
        MovableCamera& operator=(MovableCamera&& other)      = delete;

        virtual void updateUI() override;
        virtual void update(float dt) override;

        virtual void setSpeed(float speed)             { m_speed = speed; }
        virtual void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

        virtual float getSpeed() const       { return m_speed; }
        virtual float getSensitivity() const { return m_sensitivity; }

        virtual float getFOV() const         { return m_fov; }
        virtual float getAspectRatio() const { return m_aspectRatio; }
        virtual float getZNear() const       { return m_zNear; }
        virtual float getZFar() const        { return m_zFar; }

        virtual void setProjection(float fov, float aspectRatio, float zNear, float zFar);

    protected:
        virtual void mouseMovementCallback(float xpos, float ypos) override;
        virtual void mouseButtonPressedCallback(int button) override;
        virtual void mouseButtonReleaseCallback(int button) override;

        Windows::GLFWWindow* m_window = nullptr;

        float m_fov         = 45.0f;
        float m_aspectRatio = 0.0f;
        float m_zNear       = 0.1f;
        float m_zFar        = 100.0f;
        float m_speed       = 0.01f;
        float m_sensitivity = 10.0f;

    private:
        MovableCamera() { }
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_CAMERA_HPP_
