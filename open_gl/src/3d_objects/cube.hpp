#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_3D_OBJECTS_CUBE_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_3D_OBJECTS_CUBE_HPP_

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
    class Camera;

namespace Objects3D
{
    class Cube : public VertexObject
    {
    public:
        Cube() { }
        virtual ~Cube() { if (m_isInit) release(); }

        virtual void init() override;
        virtual void release() override;
        virtual void update(float dt) override;
        virtual void draw(Camera* camera) override;

    private:
        Shader m_shader;

        std::string m_texturePath = getFileFullPath("resources\\textures\\container2.png");

        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_scale    = glm::vec3(1.0f);
        glm::vec3 m_rotation = glm::vec3(0.0);

        bool m_isInit = false;
    };
}
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_3D_OBJECTS_CUBE_HPP_
