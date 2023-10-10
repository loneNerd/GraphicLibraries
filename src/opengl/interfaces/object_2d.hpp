#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_OBJECT_2D_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_OBJECT_2D_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Interfaces
{
    class IObject2D
    {
    public:
        IObject2D() { }
        virtual ~IObject2D() { }

        IObject2D(const IObject2D& other)            = delete;
        IObject2D(IObject2D&& other)                 = delete;
        IObject2D& operator=(const IObject2D& other) = delete;
        IObject2D& operator=(IObject2D&& other)      = delete;

        virtual glm::vec2 getPosition()   const { return m_position; }
        virtual glm::vec2 getScale()      const { return m_scale; }
        virtual float getRotationDegree() const { return m_rotation; }

        virtual void setPosition(glm::vec2 position) { m_position = position; }
        virtual void setScale(glm::vec2 scale)
        {
            if (scale.x <= 0.0f || scale.y <= 0.0f)
                return;

            m_scale = scale;
        }

        virtual void move(glm::vec2 newPos) { m_position += newPos; }
        virtual void scale(glm::vec2 coef) { setScale(m_scale * coef); }

        virtual void rotateXAngle(float degree)
        {
            while (degree > 360.0f)
                degree -= 360.0f;

            while (degree < 0.0f)
                degree += 360.0f;

            m_rotation = degree;
        }

        virtual glm::mat4 getModelMatrix() const
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(m_position, 0.0f));
            model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(m_scale, 1.0f));
        }

    protected:
        glm::vec2 m_position = glm::vec2(0.0f);
        glm::vec2 m_scale    = glm::vec2(1.0f);
        float m_rotation     = 0.0f;

    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_OBJECT_2D_HPP_
