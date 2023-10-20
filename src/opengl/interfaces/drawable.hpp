#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_INTERFACES_DRAWABLE_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_INTERFACES_DRAWABLE_HPP_

#include <list>
#include <memory>

#include "types/mesh.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{

namespace Interfaces { class ICamera; }

namespace Interfaces
{
    class IDrawable
    {
    public:
        IDrawable() { }
        virtual ~IDrawable() { }

        IDrawable(const IDrawable& other)            = delete;
        IDrawable(IDrawable&& other)                 = delete;
        IDrawable& operator=(const IDrawable& other) = delete;
        IDrawable& operator=(IDrawable&& other)      = delete;

        virtual void init()           = 0;
        virtual void release()        = 0;
        virtual void updateUI()       = 0;
        virtual void update(float dt) = 0;
        virtual void draw(const std::shared_ptr<Interfaces::ICamera> camera) = 0;

        const std::list<Types::Mesh>& getMeshes() const { return m_meshes; }

    protected:
        std::list<Types::Mesh> m_meshes;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_INTERFACES_DRAWABLE_HPP_
