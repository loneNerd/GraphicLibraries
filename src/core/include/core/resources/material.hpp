#pragma once
#ifndef ENGINE_CORE_RESOURCES_MATERIAL_HPP_
#define ENGINE_CORE_RESOURCES_MATERIAL_HPP_

#include <any>
#include <filesystem>
#include <map>
#include <memory>

#include "opengl/resources/shader.hpp"
#include "opengl/resources/texture.hpp"

namespace Engine::Core::ResourceManagement { class MaterialManager; }

namespace Engine::Core::Resources
{
    class Material
    {
    public:
        ~Material();

        void Bind(const std::shared_ptr<OpenGL::Resources::Shader> shader);
        void UnBind(const std::shared_ptr<OpenGL::Resources::Shader> shader);

        template<typename T> void Set(const std::string& key, const T& value);
        template<typename T> const T& Get(const std::string& key);

        void SetDepthTest(bool depthTest);
        void SetDiffuse(std::string name);
        bool HasDepthTest() const;
        uint8_t GenerateStateMask() const;
        std::optional<std::string> GetDiffuse() const { return m_diffuse; }

        const std::string Name;

    private:
        friend class Engine::Core::ResourceManagement::MaterialManager;

        Material(std::string name) : Name(name) { }

        static std::shared_ptr<Material> createSharedPtr(std::string name);
        static std::unique_ptr<Material> createUniquePtr(std::string name);

        bool m_depthTest = true;

        std::optional<std::string> m_diffuse = std::nullopt;
    };
}

#include "material.inl"

#endif // ENGINE_CORE_RESOURCES_MATERIAL_HPP_
