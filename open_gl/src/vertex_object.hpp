#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_VERTEX_OBJECT_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_VERTEX_OBJECT_HPP_

#include <list>

#include "mesh.hpp"

namespace GraphicLibraries
{
namespace Engines
{
    class Camera;

    class VertexObject
    {
    public:
        VertexObject() { }
        virtual ~VertexObject() { }

        virtual void init()               = 0;
        virtual void release()            = 0;
        virtual void update(float dt)     = 0;
        virtual void draw(Camera* camera) = 0;

    protected:
        std::list<Mesh> m_meshes;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_VERTEX_OBJECT_HPP_
