#pragma once
#ifndef GRAPHICLIBRARIES_OPEN_GL_VERTEX_HPP_
#define GRAPHICLIBRARIES_OPEN_GL_VERTEX_HPP_

#include <list>

#include <glm/glm.hpp>

namespace GraphicLibraries
{
namespace Engines
{
    struct Vertex
    {
        //Vertex() : Position { glm::vec3(0.0f) }, TextureCoord{ glm::vec2(0.0f) }{ }
        Vertex(glm::vec3 position, glm::vec2 texCoord) : 
            Position { position },
            TextureCoord { texCoord } { }

        Vertex() : Vertex(glm::vec3(0.0f), glm::vec2(0.0f)) { }

        glm::vec3 Position;
        glm::vec2 TextureCoord;
    };
}
}

#endif // GRAPHICLIBRARIES_OPEN_GL_VERTEX_HPP_
