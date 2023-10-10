#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_TYPES_MESH_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_TYPES_MESH_HPP_

#include <list>
#include <memory>
#include <vector>

#include "objects/texture.hpp"
#include "vertex.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Types
{
    struct Mesh
    {
        std::vector<Vertex> Vertices;
        std::vector<unsigned> Triangles;
        std::list<std::shared_ptr<Objects::Texture>> Textures;
        unsigned VAO;
        unsigned VBO;
        unsigned EBO;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_TYPES_MESH_HPP_
