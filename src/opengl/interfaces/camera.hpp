#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_CAMERA_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_CAMERA_HPP_

#include <glm/gtc/matrix_transform.hpp>

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Interfaces
{
    class ICamera
    {
    public:
        ICamera() { }
        virtual ~ICamera() { }

        ICamera(const ICamera& other)            = delete;
        ICamera(ICamera&& other)                 = delete;
        ICamera& operator=(const ICamera& other) = delete;
        ICamera& operator=(ICamera&& other)      = delete;

        virtual void update(float dt) = 0;

        virtual void setPosition(glm::vec3 pos)    { m_position = pos; }
        virtual void setWorldUp(glm::vec3 worldUp) { m_worldUp = worldUp; updateVectors(); }

        virtual void setYaw(float yaw)
        {
            m_yaw = yaw;

            while (m_yaw > 360.0f)
                m_yaw -= 360.0f;

            while (m_yaw < 0.0f)
                m_yaw += 360.0f;

            updateVectors();
        }

        virtual void setPitch(float pitch)
        {
            m_pitch = pitch;

            while (m_pitch > 360.0f)
                m_pitch -= 360.0f;

            while (m_pitch < 0.0f)
                m_pitch += 360.0f;

            updateVectors();
        }

        virtual void setProjection(glm::mat4 projection) { m_projection = projection; }

        virtual void movePosition(glm::vec3 newPos) { m_position += newPos; }

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        virtual glm::mat4 getViewMatrix() const
        {
            return glm::lookAt(m_position, m_position + m_front, m_up);
        }

        virtual glm::mat4 getProjection() const { return m_projection; }
        virtual glm::vec3 getPosition()   const { return m_position; }
        virtual glm::vec3 getFront()      const { return m_front; }
        virtual glm::vec3 getUp()         const { return m_up; }
        virtual glm::vec3 getRight()      const { return m_right; }
        virtual glm::vec3 getWorldUp()    const { return m_worldUp; }

        virtual float getYaw()   const { return m_yaw; }
        virtual float getPitch() const { return m_pitch; }

        virtual void updateVectors()
        {
            // calculate the new Front vector
            glm::vec3 front = glm::vec3(0.0f);
            front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            front.y = sin(glm::radians(m_pitch));
            front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            m_right = glm::normalize(glm::cross(m_front, m_worldUp));
            m_up = glm::normalize(glm::cross(m_right, m_front));
        }

    protected:
        // euler Angles
        float m_yaw   = 0.0f;
        float m_pitch = 0.0f;

        glm::mat4 m_projection = glm::mat4(1.0f);

        // camera Attributes
        glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_front    = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_up       = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_right    = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_worldUp  = glm::vec3(0.0f, 0.0f, 0.0f);
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_CAMERA_HPP_
