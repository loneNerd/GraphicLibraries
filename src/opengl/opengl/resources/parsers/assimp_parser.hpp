#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_PARSERS_ASSIMP_PARSER_HPP_
#define ENGINE_OPENGL_RESOURCES_PARSERS_ASSIMP_PARSER_HPP_

#include <filesystem>
#include <optional>

#include <assimp/Importer.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>

#include "opengl/geometry/vertex.hpp"
#include "opengl/resources/mesh.hpp"
#include "opengl/resources/parsers/model_parser_interface.hpp"

namespace Engine
{
namespace OpenGL
{
namespace Resources
{
namespace Parsers
{
    class AssimpParser : public IModelParser
    {
    public:
        bool LoadModel(const std::filesystem::path& filePath, std::shared_ptr<Model>& model, EModelParserFlags parserFlags) override;

    private:
        std::optional<std::string> createMaterial(const struct aiScene* scene, int index);
        void processNode(aiMatrix4x4& transform, struct aiNode* node, const struct aiScene* scene, std::list<std::shared_ptr<Engine::OpenGL::Resources::Mesh>>& meshes);
        void processMesh(const aiMatrix4x4& transform, struct aiMesh* mesh, const struct aiScene* scene, std::vector<Engine::OpenGL::Geometry::Vertex>& outVertices, std::vector<uint32_t>& outIndices);

        std::filesystem::path m_filePath = "";
    };
}
}
}
}

#endif // ENGINE_OPENGL_RESOURCES_PARSERS_ASSIMP_PARSER_HPP_
