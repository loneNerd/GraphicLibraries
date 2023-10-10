#include "resource_manager.hpp"

using namespace GraphicLibraries::OpenGL;

std::unordered_map<std::string, std::shared_ptr<Interfaces::IResource>> ResourceManager::m_resources;
std::list<std::string> ResourceManager::m_garbage;

void ResourceManager::collectGarbage()
{
    m_garbage.remove_if([&](const std::string& garbage)
        {
            if (m_resources.find(garbage) == m_resources.end())
                return true;

            if (m_resources[garbage].use_count() < 2)
            {
                m_resources[garbage]->release();
                m_resources[garbage].reset();
                m_resources.erase(garbage);

                return true;
            }

            return false;
        });
}

void ResourceManager::clear()
{
    static int counter = 1;

    for (auto& resource : m_resources)
    {
        resource.second->release();
        resource.second.reset();
    }

    m_resources.clear();
}
