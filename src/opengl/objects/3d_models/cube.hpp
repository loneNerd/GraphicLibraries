#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_3D_MODELS_CUBE_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_3D_MODELS_CUBE_HPP_

#include <map>
#include <string>

#include <GL/glew.h>

#include "interfaces/drawable.hpp"
#include "interfaces/object_3d.hpp"
#include "objects/shader.hpp"
#include "objects/texture.hpp"
#include "utils.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{
namespace Models3D
{
    class Cube : public Interfaces::IDrawable, public Interfaces::IObject3D
    {
    public:
        Cube() { }
        virtual ~Cube() { if (m_isInit) release(); }

        Cube(const Cube& other)            = delete;
        Cube(Cube&& other)                 = delete;
        Cube& operator=(const Cube& other) = delete;
        Cube& operator=(Cube&& other)      = delete;

        virtual void init() override;
        virtual void release() override;
        virtual void updateUI() override;
        virtual void update(float dt) override;
        virtual void draw(const std::shared_ptr<Interfaces::ICamera> camera) override;

    private:
        Objects::Shader m_shader;

        std::string m_texturePath = getFileFullPath("resources\\textures\\container2.png");

        bool m_isInit = false;
    };
}
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_3D_MODELS_CUBE_HPP_
