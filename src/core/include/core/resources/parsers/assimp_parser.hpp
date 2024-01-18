#pragma once
#ifndef ENGINE_CORE_RESOURCES_PARSERS_ASSIMP_PARSER_HPP_
#define ENGINE_CORE_RESOURCES_PARSERS_ASSIMP_PARSER_HPP_

#include <filesystem>
#include <optional>

#include <assimp/Importer.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>

#include "core/resources/parsers/model_parser_interface.hpp"
#include "opengl/geometry/vertex.hpp"

namespace Engine
{

namespace OpenGL::Resources { class Mesh;  class Model; }

namespace Core
{
namespace Resources
{
namespace Parsers
{
    class AssimpParser : public IModelParser
    {
    public:
        bool LoadModel(const std::filesystem::path& filePath, std::shared_ptr<OpenGL::Resources::Model>& model, EAssimpParserFlags assimpFlags) override;

    private:
        std::optional<std::string> createMaterial(const struct aiScene* scene, int index);
        void processNode(aiMatrix4x4& transform, struct aiNode* node, const struct aiScene* scene, std::list<std::shared_ptr<OpenGL::Resources::Mesh>>& meshes);
        void processMesh(const aiMatrix4x4& transform, struct aiMesh* mesh, const struct aiScene* scene, std::vector<OpenGL::Geometry::Vertex>& outVertices, std::vector<uint32_t>& outIndices);

        std::filesystem::path m_filePath = "";
    };
}
}
}
}

#endif // ENGINE_CORE_RESOURCES_PARSERS_ASSIMP_PARSER_HPP_
