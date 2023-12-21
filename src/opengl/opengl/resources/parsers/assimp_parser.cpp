#include "assimp_parser.hpp"

#include "core/resource_management/material_manager.hpp"
#include "core/resource_management/texture_manager.hpp"
#include "opengl/resources/model.hpp"
#include "tools/utils/path_parser.hpp"

namespace ResManagement = Engine::Core::ResourceManagement;
namespace OGLResources = Engine::OpenGL::Resources;
namespace CoreResources = Engine::Core::Resources;
namespace Parsers = OGLResources::Parsers;
namespace Geometry = Engine::OpenGL::Geometry;

namespace FS = std::filesystem;

bool Parsers::AssimpParser::LoadModel(const FS::path& filePath, std::shared_ptr<OGLResources::Model>& model, EModelParserFlags parserFlags)
{
    m_filePath = filePath;

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(filePath.string(), static_cast<unsigned int>(parserFlags));

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        return false;

    aiMatrix4x4 identity;
    processNode(identity, scene->mRootNode, scene, model->m_meshes);

    return true;
}

std::optional<std::string> Parsers::AssimpParser::createMaterial(const aiScene* scene, int index)
{
    aiMaterial* material = scene->mMaterials[index];
    if (material)
    {
        aiString name;
        aiGetMaterialString(material, AI_MATKEY_NAME, &name);
        std::string nameStr = Engine::Tools::Utils::PathParser::GetFileName(m_filePath) + "_" + name.C_Str() + ".mat";
        std::shared_ptr<CoreResources::Material> newMat = ResManagement::MaterialManager::Instance().Create(nameStr);

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString diffuse;
            aiGetMaterialString(material, AI_MATKEY_TEXTURE_DIFFUSE(0), &diffuse);
            FS::path diffusePath = m_filePath.parent_path() / diffuse.C_Str();
            ResManagement::TextureManager::Instance().Create(diffusePath, Settings::ETextureFilteringMode::LinearMipmapLinear, Settings::ETextureFilteringMode::Linear, true);
            newMat->SetDiffuse(diffusePath.string());
        }

        return nameStr;
    }

    return std::nullopt;
}

void Parsers::AssimpParser::processNode(aiMatrix4x4& transform, aiNode* node, const aiScene* scene, std::list<std::shared_ptr<OGLResources::Mesh>>& meshes)
{
    aiMatrix4x4 nodeTransformation = transform * node->mTransformation;

    // Process all the node's meshes (if any)
    for (uint32_t i = 0; i < node->mNumMeshes; ++i)
    {
        std::vector<Geometry::Vertex> vertices;
        std::vector<uint32_t> indices;
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(nodeTransformation, mesh, scene, vertices, indices);
        std::optional<std::string> matName = createMaterial(scene, mesh->mMaterialIndex);
        // The model will handle mesh destruction
        meshes.emplace_back(std::make_shared<Mesh>(vertices, indices, matName.value()));
    }

    // Then do the same for each of its children
    for (uint32_t i = 0; i < node->mNumChildren; ++i)
    {
        processNode(nodeTransformation, node->mChildren[i], scene, meshes);
    }
}

void Parsers::AssimpParser::processMesh(const aiMatrix4x4& transform, aiMesh* mesh, const aiScene* scene, std::vector<Geometry::Vertex>& outVertices, std::vector<uint32_t>& outIndices)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
    {
        aiVector3D position = transform * mesh->mVertices[i];
        aiVector3D normal = transform * (mesh->mNormals ? mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
        aiVector3D texCoords = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
        aiVector3D tangent = mesh->mTangents ? transform * mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
        aiVector3D bitangent = mesh->mBitangents ? transform * mesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);

        outVertices.push_back
        ({
            { position.x,  position.y, position.z },
            { texCoords.x, texCoords.y },
            { normal.x,    normal.y,    normal.z },
            { tangent.x,   tangent.y,   tangent.z },
            { bitangent.x, bitangent.y, bitangent.z }
        });
    }

    for (uint32_t faceID = 0; faceID < mesh->mNumFaces; ++faceID)
    {
        aiFace face = mesh->mFaces[faceID];

        for (size_t indexID = 0; indexID < face.mNumIndices; ++indexID)
            outIndices.push_back(face.mIndices[indexID]);
    }
}
