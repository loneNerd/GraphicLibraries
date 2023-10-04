#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_CAMERA_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_CAMERA_HPP_

#include <iostream>

#include <glm/glm.hpp>

#include "windows/window_observers.hpp"

namespace GraphicLibraries
{

namespace Windows { class IWindow; }

namespace Engines
{
    class Camera : public Windows::IInputObserver
    {
    public:
        Camera(Windows::IWindow* window = nullptr);
        virtual ~Camera();

        Camera(const Camera& other)            = delete;
        Camera(Camera&& other)                 = delete;
        Camera& operator=(const Camera& other) = delete;
        Camera& operator=(Camera&& other)      = delete;

        virtual void update(float dt);

        virtual void setPosition( glm::vec3 pos )    { m_position = pos; }
        virtual void setWorldUp( glm::vec3 worldUp ) { m_worldUp = worldUp; updateVectors(); }

        virtual void setYaw( float yaw )
        {
            m_yaw = yaw;

            while ( m_yaw > 360.0f )
                m_yaw -= 360.0f;

            while ( m_yaw < 0.0f )
                m_yaw += 360.0f; 

            updateVectors();
        }

        virtual void setPitch( float pitch )
        {
            m_pitch = pitch;

            while ( m_pitch > 360.0f )
                m_pitch -= 360.0f;

            while ( m_pitch < 0.0f )
                m_pitch += 360.0f; 

            updateVectors();
        }

        virtual void setSpeed( float speed ) { m_speed = speed; }
        virtual void setSensitivity( float sensitivity ) { m_sensitivity = sensitivity; }

        virtual void movePosition( glm::vec3 newPos ) { m_position += newPos; }

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        virtual glm::mat4 getViewMatrix() const;

        virtual glm::mat4 getProjection() const { return m_projection; }
        virtual glm::vec3 getPosition()   const { return m_position; }
        virtual glm::vec3 getFront()      const { return m_front; }
        virtual glm::vec3 getUp()         const { return m_up; }
        virtual glm::vec3 getRight()      const { return m_right; }
        virtual glm::vec3 getWorldUp()    const { return m_worldUp; }

        virtual float getYaw()   const { return m_yaw; }
        virtual float getPitch() const { return m_pitch; }

        virtual float getSpeed() const { return m_speed; }
        virtual float getSensitivity() const { return m_sensitivity; }

        virtual void setProjection( glm::mat4 projection ) { m_projection = projection; }
        virtual void setOrthoProjection( float left, float right, float bottom, float top, float zNear, float zFar );
        virtual void setPerspectiveProjection( float fov, float aspectRatio, float zNear, float zFar );

        virtual void updateVectors();

    protected:
        virtual void mouseMovementCallback(float xpos, float ypos) override;
        virtual void mouseButtonPressedCallback(int button) override;
        virtual void mouseButtonReleaseCallback(int button) override;

        Windows::IWindow* m_window = nullptr;

        glm::mat4 m_projection = glm::mat4( 1.0f );

        // camera Attributes
        glm::vec3 m_position = glm::vec3( 0.0f, 0.0f, 0.0f );
        glm::vec3 m_front    = glm::vec3( 0.0f, 0.0f, 0.0f );
        glm::vec3 m_up       = glm::vec3( 0.0f, 0.0f, 0.0f );
        glm::vec3 m_right    = glm::vec3( 0.0f, 0.0f, 0.0f );
        glm::vec3 m_worldUp  = glm::vec3( 0.0f, 0.0f, 0.0f );

        // euler Angles
        float m_yaw   = 0.0f;
        float m_pitch = 0.0f;
        float m_speed = 0.01f;
        float m_sensitivity = 10.0f;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_CAMERA_HPP_
