#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_2D_OBJECTS_TRIANGLE_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_2D_OBJECTS_TRIANGLE_HPP_

#include <map>
#include <string>

#include <gl/glew.h>

#include "shader.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "vertex_object.hpp"

namespace GraphicLibraries
{
namespace Engines
{
namespace Objects2D
{
    class Triangle : VertexObject
    {
    public:
        Triangle() { }
        virtual ~Triangle() { if (m_isInit) release(); }

        virtual void init() override;
        virtual void release() override;
        virtual void update(float dt) override;
        virtual void draw() override;

    private:
        enum class EColorType
        {
            ESingleColor,
            ESimpleTexture
        };

        std::map<EColorType, Shader> m_shaders;

        EColorType m_currentColorType = EColorType::ESingleColor;

        std::string m_texturePath = getFileFullPath("resources\\textures\\wood.png");
        Texture m_texture;

        glm::vec4 m_color    = glm::vec4(0.0f, 0.75f, 0.0f, 1.0f);
        glm::vec2 m_position = glm::vec2(0.0f);
        glm::vec2 m_size     = glm::vec2(1.0f);
        float m_rotation     = 0.0f;

        bool m_isInit = false;
    };
}
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_2D_OBJECTS_TRIANGLE_HPP_
