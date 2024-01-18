#pragma once
#ifndef ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_MANAGER_HPP_
#define ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_MANAGER_HPP_

#include <any>
#include <filesystem>
#include <memory>
#include <unordered_map>

namespace Engine
{
namespace Core
{
namespace ResourceManagement
{
    template<typename T>
    class ResourceManager
    {
    public:
        virtual ~ResourceManager()
        {
            RemoveResources();
        }

        ResourceManager(const ResourceManager& other)             = delete;
        ResourceManager(ResourceManager&& other)                  = delete;
        ResourceManager& operator=(const ResourceManager& other)  = delete;
        ResourceManager& operator=(const ResourceManager&& other) = delete;

        virtual bool IsResourceLoaded(const std::string& name)
        {
            return !m_resources.empty() && m_resources.find(name) != m_resources.end();
        }

        virtual T GetResource(const std::string& name)
        {
            auto resource = m_resources.find(name);

            if (resource != m_resources.end())
                return resource->second;

            return nullptr;
        }

        virtual T operator[](const std::string& name)
        {
            return GetResource(name);
        }

        virtual void RemoveResources()
        {
            for (auto& value : m_resources)
                value.second = nullptr;

            m_resources.clear();
        }

        static void SetAssetPaths(std::filesystem::path assetsPath)
        {
            AssetPath = assetsPath;
        }

        virtual std::filesystem::path GetRealPath(const std::filesystem::path& path) const { return AssetPath / path; }

    protected:
        ResourceManager() = default;

        virtual bool RenameResource(const std::string& oldName, std::string newName)
        {
            auto oldIt = m_resources.find(oldName);
            auto newIt = m_resources.find(newName);

            if (oldIt != m_resources.end() && newIt == m_resources.end())
            {
                std::swap(newIt, oldIt);
                m_resources.erase(oldIt);
                return true;
            }

            return false;
        }

        virtual void RemoveResource(const std::string& name)
        {
            if (IsResourceLoaded(name))
            {
                m_resources[name] = nullptr;
                m_resources.erase(name);
            }
        }

        inline static std::filesystem::path AssetPath = "";
        std::unordered_map<std::string, T> m_resources;
    };
}
}
}

#endif // ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_MANAGER_HPP_
