#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_SHADER_HPP_
#define ENGINE_OPENGL_RESOURCES_SHADER_HPP_

#include <filesystem>
#include <optional>
#include <unordered_map>

#include "math/matrix4.hpp"
#include "math/vector2.hpp"
#include "uniform_info.hpp"

//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>

namespace Engine::Core::ResourceManagement { class ShaderManager; }

namespace Engine
{
namespace OpenGL
{
namespace Resources
{
    namespace Loaders { class ShaderLoader; }

    class Shader
    {
    public:
        ~Shader();

        Shader(const Shader& other)             = delete;
        Shader(Shader&& other)                  = delete;
        Shader& operator=(const Shader& other)  = delete;
        Shader& operator=(const Shader&& other) = delete;

        void Bind() const;
        void Unbind() const;
        void SetUniformInt(const std::string& name, int value);
        void SetUniformFloat(const std::string& name, float value);
        void SetUniformVec2(const std::string& name, const Math::FVector2& vec2);
        void SetUniformVec3(const std::string& name, const Math::FVector3& vec3);
        void SetUniformVec4(const std::string& name, const Math::FVector4& vec4);
        void SetUniformMat4(const std::string& name, const Math::FMatrix4& mat4);
        //void SetUniformMat4(const std::string& name, const glm::mat4& mat4);
        int GetUniformInt(const std::string& name);
        float GetUniformFloat(const std::string& name);
        Math::FVector2 GetUniformVec2(const std::string& name);
        Math::FVector3 GetUniformVec3(const std::string& name);
        Math::FVector4 GetUniformVec4(const std::string& name);
        Math::FMatrix4 GetUniformMat4(const std::string& name);
        std::optional<std::reference_wrapper<const UniformInfo>> GetUniformInfo(const std::string& name) const;

        const uint32_t Id;
        const std::filesystem::path VertexFileTrace;
        const std::filesystem::path FragmentFileTrace;
        const std::optional<std::filesystem::path> GeometryFileTrace;
        const std::string Name;
        std::vector<UniformInfo> Uniforms;

    private:
        friend class Engine::Core::ResourceManagement::ShaderManager;

        Shader(std::string name, std::filesystem::path vertex, std::filesystem::path fragment, std::optional<std::filesystem::path> geometry, uint32_t id);
        static std::shared_ptr<Shader> createSharedPtr(std::string name, std::filesystem::path vertex, std::filesystem::path fragment, std::optional<std::filesystem::path> geometry, uint32_t id);
        static std::unique_ptr<Shader> createUniquePtr(std::string name, std::filesystem::path vertex, std::filesystem::path fragment, std::optional<std::filesystem::path> geometry, uint32_t id);

        void queryUniforms();
        uint32_t getUniformLocation(const std::string& name);

        std::unordered_map<std::string, int> m_uniformLocationCache;
    };
}
}
}

#endif // ENGINE_OPENGL_RESOURCES_SHADER_HPP_
