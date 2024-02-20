#pragma once
#ifndef ENGINE_CORE_RESOURCE_MANAGEMENT_MATERIAL_MANAGER_HPP_
#define ENGINE_CORE_RESOURCE_MANAGEMENT_MATERIAL_MANAGER_HPP_

#include <filesystem>
#include <memory>

#include "resource_manager.hpp"

#include "core/resources/material.hpp"

namespace Engine::Core::ResourceManagement
{
    class MaterialManager : public ResourceManager<std::shared_ptr<Core::Resources::Material>>
    {
    public:
        virtual ~MaterialManager()
        {
            RemoveResources();
        }

        MaterialManager(const MaterialManager& other) = delete;
        MaterialManager(MaterialManager&& other) = delete;
        MaterialManager& operator=(const MaterialManager& other) = delete;
        MaterialManager& operator=(const MaterialManager&& other) = delete;

        static MaterialManager& Instance()
        {
            static MaterialManager instance;
            return instance;
        }

        virtual std::shared_ptr<Core::Resources::Material> Create(std::string name);
        virtual void Destroy(const std::string& name);

    private:
        MaterialManager() = default;
    };
}

#endif // ENGINE_CORE_RESOURCE_MANAGEMENT_MATERIAL_MANAGER_HPP_
