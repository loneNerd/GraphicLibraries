#include "core/resource_management/material_manager.hpp"

namespace FS = std::filesystem;

namespace ResManagement = Engine::Core::ResourceManagement;
namespace Resources = Engine::Core::Resources;

std::shared_ptr<Resources::Material> ResManagement::MaterialManager::Create(std::string name)
{
    if (IsResourceLoaded(name))
        return GetResource(name);

    auto newMaterial = m_resources.emplace(name, Resources::Material::createSharedPtr(name));
    return newMaterial.first->second;
}

void ResManagement::MaterialManager::Destroy(const std::string& name)
{
    if (IsResourceLoaded(name) && m_resources[name].use_count() < 2)
    {
        RemoveResource(name);
    }
}
