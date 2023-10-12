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
    class MovebleCamera : public Interfaces::ICamera, public Windows::IInputObserver
    {
    public:
        MovebleCamera(std::shared_ptr<Windows::GLFWWindow> window);
        virtual ~MovebleCamera();

        MovebleCamera(const MovebleCamera& other)            = delete;
        MovebleCamera(MovebleCamera&& other)                 = delete;
        MovebleCamera& operator=(const MovebleCamera& other) = delete;
        MovebleCamera& operator=(MovebleCamera&& other)      = delete;

        virtual void update(float dt) override;

        virtual void setSpeed(float speed) { m_speed = speed; }
        virtual void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

        virtual float getSpeed() const { return m_speed; }
        virtual float getSensitivity() const { return m_sensitivity; }

        virtual void setProjection(float fov, float aspectRatio, float zNear, float zFar);

    protected:
        virtual void mouseMovementCallback(float xpos, float ypos) override;
        virtual void mouseButtonPressedCallback(int button) override;
        virtual void mouseButtonReleaseCallback(int button) override;

        std::shared_ptr<Windows::GLFWWindow> m_window = nullptr;

        float m_speed       = 0.01f;
        float m_sensitivity = 10.0f;

    private:
        MovebleCamera() { }
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_CAMERA_HPP_
