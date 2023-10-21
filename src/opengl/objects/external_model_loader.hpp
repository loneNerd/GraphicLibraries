#pragma once
#ifndef GRAPHIC_LIBRARIES_OPENGL_OBJECTS_EXTERNAL_MODEL_LOADER_HPP_
#define GRAPHIC_LIBRARIES_OPENGL_OBJECTS_EXTERNAL_MODEL_LOADER_HPP_

#include <list>
#include <memory>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "types/mesh.hpp"

namespace GraphicLibraries
{
namespace OpenGL
{
namespace Objects
{
    class ExternalModelLoader
    {
    public:
        ExternalModelLoader() { }
        ~ExternalModelLoader() { }

        std::list<Types::Mesh> load(const char* path);

    private:
        std::list<Types::Mesh> processNode(aiNode* node, const aiScene* scene);
        Types::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::list<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type);

        std::string m_fileName = "";
    };
}
}
}

#endif // GRAPHIC_LIBRARIES_OPENGL_OBJECTS_EXTERNAL_MODEL_LOADER_HPP_
