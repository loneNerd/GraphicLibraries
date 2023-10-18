#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_2D_MODELS_TRIANGLE_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_2D_MODELS_TRIANGLE_HPP_

#include <map>
#include <string>

#include <GL/glew.h>

#include "interfaces/drawable.hpp"
#include "interfaces/object_2d.hpp"
#include "objects/shader.hpp"
#include "objects/texture.hpp"
#include "utils.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{
namespace Models2D
{
    class Triangle : public Interfaces::IDrawable, public Interfaces::IObject2D
    {
    public:
        Triangle() { }
        virtual ~Triangle() { if (m_isInit) release(); }

        Triangle(const Triangle& other)            = delete;
        Triangle(Triangle&& other)                 = delete;
        Triangle& operator=(const Triangle& other) = delete;
        Triangle& operator=(Triangle&& other)      = delete;

        virtual void init() override;
        virtual void release() override;
        virtual void update(float dt) override;
        virtual void draw(const std::shared_ptr<Interfaces::ICamera> camera) override;

    private:
        enum class EColorType
        {
            ESingleColor,
            ESimpleTexture
        };

        std::map<EColorType, Objects::Shader> m_shaders;

        EColorType m_currentColorType = EColorType::ESingleColor;

        std::string m_texturePath = getFileFullPath("resources\\textures\\wood.png");

        glm::vec4 m_color = glm::vec4(0.0f, 0.75f, 0.0f, 1.0f);

        bool m_isInit = false;
    };
}
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_2D_MODELS_TRIANGLE_HPP_
