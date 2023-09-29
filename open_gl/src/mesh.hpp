#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_MESH_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_MESH_HPP_

#include <list>
#include <memory>
#include <vector>

#include "texture.hpp"
#include "vertex.hpp"

namespace GraphicLibraries
{
namespace Engines
{
    struct Mesh
    {
        std::vector<Vertex> Vertices;
        std::vector<unsigned> Triangles;
        std::list<std::shared_ptr<Texture>> Textures;
        unsigned VAO;
        unsigned VBO;
        unsigned EBO;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_VERTEX_OBJECT_HPP_
