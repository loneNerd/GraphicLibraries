#pragma once
#ifndef ENGINE_CORE_RESOURCE_MANAGEMENT_TEXTURE_MANAGER_HPP_
#define ENGINE_CORE_RESOURCE_MANAGEMENT_TEXTURE_MANAGER_HPP_

#include <filesystem>

#include "opengl/resources/texture.hpp"
#include "resource_manager.hpp"

namespace Engine::Core::ResourceManagement
{
    class TextureManager : public ResourceManager<std::shared_ptr<OpenGL::Resources::Texture>>
    {
    public:
        virtual ~TextureManager()
        {
            RemoveResources();
        }

        TextureManager(const TextureManager& other)             = delete;
        TextureManager(TextureManager&& other)                  = delete;
        TextureManager& operator=(const TextureManager& other)  = delete;
        TextureManager& operator=(const TextureManager&& other) = delete;

        static TextureManager& Instance()
        {
            static TextureManager instance;
            return instance;
        }

        virtual std::shared_ptr<OpenGL::Resources::Texture> Create(std::filesystem::path path, OpenGL::Settings::ETextureFilteringMode firstFilter, OpenGL::Settings::ETextureFilteringMode secondFilter, bool generateMipmap, bool flip = false);
        virtual std::shared_ptr<OpenGL::Resources::Texture> CreateColor(std::string name, uint32_t data, OpenGL::Settings::ETextureFilteringMode firstFilter, OpenGL::Settings::ETextureFilteringMode secondFilter, bool generateMipmap);
        virtual std::shared_ptr<OpenGL::Resources::Texture> CreateFromMemory(std::string name, uint8_t* data, uint32_t width, uint32_t height, OpenGL::Settings::ETextureFilteringMode firstFilter, OpenGL::Settings::ETextureFilteringMode secondFilter, bool generateMipmap);
        virtual void Reload(std::shared_ptr<OpenGL::Resources::Texture> texture, std::filesystem::path filePath, OpenGL::Settings::ETextureFilteringMode firstFilter, OpenGL::Settings::ETextureFilteringMode secondFilter, bool generateMipmap, bool flip = false);
        virtual void Destroy(const std::string& name);

    private:
        TextureManager() = default;
    };
}

#endif // ENGINE_CORE_RESOURCE_MANAGEMENT_TEXTURE_MANAGER_HPP_
