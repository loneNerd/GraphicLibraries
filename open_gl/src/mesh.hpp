#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_MESH_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_MESH_HPP_

#include <vector>

#include "vertex.hpp"

namespace GraphicLibraries
{
namespace Engines
{
    struct Mesh
    {
        std::vector<Vertex> Vertices;
        std::vector<unsigned> Triangles;
        unsigned VAO;
        unsigned VBO;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_VERTEX_OBJECT_HPP_
