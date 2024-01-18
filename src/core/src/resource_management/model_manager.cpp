#include "core/resource_management/model_manager.hpp"
#include "core/resource_management/material_manager.hpp"

namespace OGLResources = Engine::OpenGL::Resources;
namespace ResManagement = Engine::Core::ResourceManagement;
namespace CoreResources = Engine::Core::Resources;
namespace Parsers = CoreResources::Parsers;

namespace FS = std::filesystem;

std::shared_ptr<OGLResources::Model> ResManagement::ModelManager::Create(FS::path filePath, Parsers::EAssimpParserFlags parserFlags)
{
    if (IsResourceLoaded(filePath.string()))
        return GetResource(filePath.string());

    auto newModel = m_resources.emplace(filePath.string(), OGLResources::Model::createSharedPtr(filePath));
    std::shared_ptr<OGLResources::Model> model = newModel.first->second;

    CoreResources::Parsers::AssimpParser assimp;
    
    if (model && assimp.LoadModel(filePath, model, parserFlags))
    {
        return model;
    }

    return nullptr;
}

void ResManagement::ModelManager::Reload(std::shared_ptr<OGLResources::Model> model, FS::path filePath, Parsers::EAssimpParserFlags parserFlags)
{
    RemoveResource(model->Path.string());
    std::shared_ptr<OGLResources::Model> newModel = Create(filePath, parserFlags);
    model = newModel;
}

void ResManagement::ModelManager::Destroy(const std::string& name)
{
    if (IsResourceLoaded(name) && m_resources[name].use_count() < 2)
    {
        RemoveResource(name);
    }
}
