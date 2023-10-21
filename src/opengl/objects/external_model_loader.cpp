#include "external_model_loader.hpp"

#include <stdexcept>

#include "objects/texture.hpp"
#include "resource_manager.hpp"

using namespace GraphicLibraries::OpenGL::Objects;
using namespace GraphicLibraries::OpenGL::Types;

std::list<Mesh> ExternalModelLoader::load(const char* path)
{
    m_fileName = std::string(path);

    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
                                             aiProcess_Triangulate           |
                                             aiProcess_GenSmoothNormals      |
                                             aiProcess_FlipUVs               |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        throw std::runtime_error(std::string("ASSIMP: ").append(importer.GetErrorString()));

    // process ASSIMP's root node recursively
    return processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
std::list<Mesh> ExternalModelLoader::processNode(aiNode* node, const aiScene* scene)
{
    std::list<Mesh> meshes;

    // process each mesh located at the current node
    for (unsigned i = 0; i < node->mNumMeshes; ++i)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned i = 0; i < node->mNumChildren; ++i)
    {
        std::list<Mesh> nodeMeshes = processNode(node->mChildren[i], scene);
        meshes.insert(meshes.end(), nodeMeshes.begin(), nodeMeshes.end());
    }

    return meshes;
}

Mesh ExternalModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    Mesh newMesh { };

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex { };

        // positions
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        // normals
        if (mesh->HasNormals())
        {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.TextureCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.TextureCoord.y = mesh->mTextureCoords[0][i].y;

            if (mesh->HasTangentsAndBitangents())
            {
                // tangent
                vertex.Tangent.x = mesh->mTangents[i].x;
                vertex.Tangent.y = mesh->mTangents[i].y;
                vertex.Tangent.z = mesh->mTangents[i].z;

                // bitangent
                vertex.Bitangent.x = mesh->mBitangents[i].x;
                vertex.Bitangent.y = mesh->mBitangents[i].y;
                vertex.Bitangent.z = mesh->mBitangents[i].z;
            }
        }
        else
            vertex.TextureCoord = glm::vec2(0.0f, 0.0f);

        newMesh.Vertices.emplace_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned j = 0; j < face.mNumIndices; ++j)
            newMesh.Triangles.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    if (material->GetTextureCount(aiTextureType_DIFFUSE))
    {
        std::list<std::shared_ptr<Objects::Texture>> diffuses = loadMaterialTextures(material, aiTextureType_DIFFUSE);
        newMesh.Textures.insert(newMesh.Textures.end(), diffuses.begin(), diffuses.end());
    }

    //vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //// 2. specular maps
    //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //// 3. normal maps
    //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //// 4. height maps
    //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return newMesh;
}

std::list<std::shared_ptr<Texture>> ExternalModelLoader::loadMaterialTextures(aiMaterial* material, aiTextureType type)
{
    std::list<std::shared_ptr<Texture>> textures;

    for (unsigned i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString path;
        material->GetTexture(type, i, &path);

        switch (type)
        {
            case aiTextureType_DIFFUSE:
            {
                std::shared_ptr<Texture> texture = ResourceManager::loadResource(m_fileName.substr(0, m_fileName.find_last_of('\\') + 1).append(path.C_Str()).c_str());
                texture->setTextureType(ETextureType::EDiffuse);
                textures.push_back(texture);
                break;
            }
            default:
            {
                throw std::runtime_error("Unsupported texture type");
                break;
            }
        }
    }

    return textures;
}
