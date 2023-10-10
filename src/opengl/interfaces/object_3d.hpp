#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_OBJECT_3D_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_OBJECT_3D_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Interfaces
{
    class IObject3D
    {
    public:
        IObject3D() { }
        virtual ~IObject3D() { }

        IObject3D(const IObject3D& other)            = delete;
        IObject3D(IObject3D&& other)                 = delete;
        IObject3D& operator=(const IObject3D& other) = delete;
        IObject3D& operator=(IObject3D&& other)      = delete;

        //virtual const glm::vec3 getSize() const { return m_size * m_scale; }

        virtual glm::vec3 getPosition()       const { return m_position; }
        virtual glm::vec3 getScale()          const { return m_scale; }
        virtual glm::vec3 getRotationQuad()   const { return glm::radians(m_rotation); }
        virtual glm::vec3 getRotationDegree() const { return m_rotation; }
        //virtual float getXAngle() const { return glm::degrees(m_rotation.x); }
        //virtual float getYAngle() const { return glm::degrees(m_rotation.y); }
        //virtual float getZAngle() const { return glm::degrees(m_rotation.z); }

        virtual void setPosition(glm::vec3 position) { m_position = position; }
        virtual void setScale(glm::vec3 scale)
        {
            if (scale.x <= 0.0f || scale.y <= 0.0f || scale.z <= 0.0f)
                return;

            m_scale = scale;
        }

        virtual void move(glm::vec3 newPos) { m_position += newPos; }
        virtual void scale(glm::vec3 coef)  { setScale(m_scale * coef); }

        virtual void rotateQuad(glm::vec3 theta)
        {
            rotateXAngle(m_rotation.x + glm::degrees(theta.x));
            rotateYAngle(m_rotation.y + glm::degrees(theta.y));
            rotateZAngle(m_rotation.z + glm::degrees(theta.z));
        }

        virtual void rotateXAngle(float degree)
        {
            while (degree > 360.0f)
                degree -= 360.0f;

            while (degree < 0.0f)
                degree += 360.0f;

            m_rotation.x = degree;
        }

        virtual void rotateYAngle(float degree)
        {
            while (degree > 360.0f)
                degree -= 360.0f;

            while (degree < 0.0f)
                degree += 360.0f;

            m_rotation.y = degree;
        }

        virtual void rotateZAngle(float degree)
        {
            while (degree > 360.0f)
                degree -= 360.0f;

            while (degree < 0.0f)
                degree += 360.0f;

            m_rotation.z = degree;
        }

        virtual glm::mat4 getModelMatrix() const
        {
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, m_position + m_modelPosShift * m_scale);
            model = glm::translate(model, m_position);
            model *= glm::mat4_cast(glm::quat(glm::radians(m_rotation)));
            model = glm::scale(model, m_scale);
            return model;
        }

    protected:
        //glm::vec3 m_size     = glm::vec3(0.0f);
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_scale    = glm::vec3(1.0f);
        glm::vec3 m_rotation = glm::vec3(0.0f);

        //glm::vec3 m_modelPosShift = glm::vec3(0.0f);
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_OBJECT_3D_HPP_
