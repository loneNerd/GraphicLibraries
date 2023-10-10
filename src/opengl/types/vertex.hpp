#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_TYPES_VERTEX_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_TYPES_VERTEX_HPP_

#include <array>
#include <list>

#include <glm/glm.hpp>

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Types
{
    #define MAX_BONE_INFLUENCE 4
    struct Vertex
    {
        Vertex(glm::vec3 position = glm::vec3(0.0f),
               glm::vec2 texCoord = glm::vec2(0.0f),
               glm::vec3 normal = glm::vec3(0.0f),
               glm::vec3 tanget = glm::vec3(0.0f),
               glm::vec3 bitangent = glm::vec3(0.0f),
               std::array<int, 4> boneIDs = { 0 },
               std::array<float, 4> weights = { 0.0f }) :
            Position { position },
            TextureCoord { texCoord },
            Normal { normal },
            Tangent { tanget },
            Bitangent{ bitangent },
            BoneIDs { boneIDs },
            Weights { weights } { }

        glm::vec3 Position;
        glm::vec2 TextureCoord;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;

        std::array<int, MAX_BONE_INFLUENCE> BoneIDs;
        std::array<float, MAX_BONE_INFLUENCE> Weights;
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_TYPES_VERTEX_HPP_
