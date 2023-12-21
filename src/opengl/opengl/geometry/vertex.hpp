#pragma once
#ifndef ENGINE_OPENGL_GEOMETRY_VERTEX_HPP_
#define ENGINE_OPENGL_GEOMETRY_VERTEX_HPP_

#include <array>

namespace Engine
{
namespace OpenGL
{
namespace Geometry
{
    struct Vertex
    {
        std::array<float, 3> Position;
        std::array<float, 2> TexCoords;
        std::array<float, 3> Normals;
        std::array<float, 3> Tangent;
        std::array<float, 3> Bitangent;
    };
}
}
}

#endif // ENGINE_OPENGL_GEOMETRY_VERTEX_HPP_
