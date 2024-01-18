#pragma once
#ifndef ENGINE_CORE_RESOURCE_MANAGEMENT_SHADER_MANAGER_HPP_
#define ENGINE_CORE_RESOURCE_MANAGEMENT_SHADER_MANAGER_HPP_

#include <filesystem>

#include "opengl/resources/shader.hpp"
#include "resource_manager.hpp"

namespace Engine
{
namespace Core
{
namespace ResourceManagement
{
    class ShaderManager : public ResourceManager<std::shared_ptr<OpenGL::Resources::Shader>>
    {
    public:
        virtual ~ShaderManager()
        {
            RemoveResources();
        }

        static ShaderManager& Instance()
        {
            static ShaderManager instance;
            return instance;
        }

        virtual std::shared_ptr<OpenGL::Resources::Shader> Create(std::filesystem::path vertexShader, std::filesystem::path fragmentShader, std::optional<std::filesystem::path> geometryShader);
        virtual void Recompile(std::shared_ptr<OpenGL::Resources::Shader> shader, std::filesystem::path vertexShader, std::filesystem::path fragmentShader, std::optional<std::filesystem::path> geometryShader);
        virtual void Destroy(const std::string& name);

    private:
        ShaderManager() = default;

        uint32_t createProgram(std::filesystem::path vertexShader, std::filesystem::path fragmentShader, std::optional<std::filesystem::path> geometryShader);
        uint32_t compileShader(uint32_t type, const std::filesystem::path& path);
    };
}
}
}

#endif // ENGINE_CORE_RESOURCE_MANAGEMENT_SHADER_MANAGER_HPP_
