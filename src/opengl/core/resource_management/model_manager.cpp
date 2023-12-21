#include "model_manager.hpp"

#include "material_manager.hpp"

namespace ResManagement = Engine::Core::ResourceManagement;
namespace OGLResources = Engine::OpenGL::Resources;
namespace Parsers = OGLResources::Parsers;

namespace FS = std::filesystem;

std::shared_ptr<OGLResources::Model> ResManagement::ModelManager::Create(FS::path filePath, Parsers::EModelParserFlags parserFlags)
{
    if (IsResourceLoaded(filePath.string()))
        return GetResource(filePath.string());

    auto newModel = m_resources.emplace(filePath.string(), OGLResources::Model::createSharedPtr(filePath));
    std::shared_ptr<OGLResources::Model> model = newModel.first->second;

    OGLResources::Parsers::AssimpParser assimp;
    
    if (model && assimp.LoadModel(filePath, model, parserFlags))
    {
        return model;
    }

    return nullptr;
}

void ResManagement::ModelManager::Reload(std::shared_ptr<OGLResources::Model> model, FS::path filePath, Parsers::EModelParserFlags parserFlags)
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
