#pragma once
#ifndef ENGINE_CORE_RESOURCE_MANAGEMENT_MODEL_MANAGER_HPP_
#define ENGINE_CORE_RESOURCE_MANAGEMENT_MODEL_MANAGER_HPP_

#include <filesystem>

#include "resource_manager.hpp"

#include "opengl/resources/model.hpp"
#include "core/resources/parsers/assimp_parser.hpp"

namespace Engine
{
namespace Core
{
namespace ResourceManagement
{
    class ModelManager : public ResourceManager<std::shared_ptr<OpenGL::Resources::Model>>
    {
    public:
        ModelManager() = default;

        virtual ~ModelManager()
        {
            RemoveResources();
        }

        ModelManager(const ModelManager& other)             = delete;
        ModelManager(ModelManager&& other)                  = delete;
        ModelManager& operator=(const ModelManager& other)  = delete;
        ModelManager& operator=(const ModelManager&& other) = delete;

        static ModelManager& Instance()
        {
            static ModelManager instance;
            return instance;
        }

        virtual std::shared_ptr<OpenGL::Resources::Model> Create(std::filesystem::path filePath, Core::Resources::Parsers::EAssimpParserFlags parserFlags = Core::Resources::Parsers::EAssimpParserFlags::None);
        virtual void Reload(std::shared_ptr<OpenGL::Resources::Model> model, std::filesystem::path filePath, Core::Resources::Parsers::EAssimpParserFlags parserFlags = Core::Resources::Parsers::EAssimpParserFlags::None);
        virtual void Destroy(const std::string& name);
    };
}
}
}

#endif // ENGINE_CORE_RESOURCE_MANAGEMENT_MODEL_MANAGER_HPP_
