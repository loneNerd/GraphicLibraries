#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_MESH_INTERFACE_HPP_
#define ENGINE_OPENGL_RESOURCES_MESH_INTERFACE_HPP_

#include <stdint.h>

namespace Engine
{
namespace OpenGL
{
namespace Resources
{
    class IMesh
    {
    public:
        virtual void Bind()               = 0;
        virtual void Unbind()             = 0;
        virtual uint32_t GetVertexCount() = 0;
        virtual uint32_t GetIndexCount()  = 0;
    };
}
}
}

#endif // ENGINE_OPENGL_RESOURCES_MESH_INTERFACE_HPP_
