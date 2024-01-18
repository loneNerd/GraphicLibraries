#include "opengl/resources/model.hpp"

namespace Resources = Engine::OpenGL::Resources;

Resources::Model::~Model()
{
    for (std::shared_ptr<Mesh>& mesh : m_meshes)
        mesh = nullptr;
}

std::shared_ptr<Resources::Model> Resources::Model::createSharedPtr(std::filesystem::path path)
{
    Resources::Model* model = new Resources::Model(path);
    return std::shared_ptr<Resources::Model>(model);
}

std::unique_ptr<Resources::Model> Resources::Model::createUniquePtr(std::filesystem::path path)
{
    Resources::Model* model = new Resources::Model(path);
    return std::unique_ptr<Resources::Model>(model);
}

const std::list<std::shared_ptr<Resources::Mesh>>& Resources::Model::GetMeshes() const
{
    return m_meshes;
}
