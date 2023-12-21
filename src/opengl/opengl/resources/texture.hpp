#pragma once
#ifndef ENGINE_OPENGL_RESOURCES_TEXTURE_HPP_
#define ENGINE_OPENGL_RESOURCES_TEXTURE_HPP_

#include <string>

#include "mesh.hpp"
#include "opengl/settings/texture_filtering_mode.hpp"

namespace Engine::Core::ResourceManagement { class TextureManager; }

namespace Engine
{
namespace OpenGL
{
namespace Resources
{

    class Texture
    {
    public:
        ~Texture();

        Texture(const Texture& other)             = delete;
        Texture(Texture&& other)                  = delete;
        Texture& operator=(const Texture& other)  = delete;
        Texture& operator=(const Texture&& other) = delete;

        void Bind(uint32_t slot = 0) const;
        void Unbind() const;

        const uint32_t Id;
        const uint32_t Width;
        const uint32_t Height;
        const uint32_t BitsPerPixel;
        const Settings::ETextureFilteringMode FirstFilter;
        const Settings::ETextureFilteringMode SecondFilter;
        const std::string Name;
        const bool IsMimapped;

    private:
        friend class Engine::Core::ResourceManagement::TextureManager;

        Texture(std::string name, uint32_t id, uint32_t width, uint32_t height, uint32_t bpp, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap);

        static std::shared_ptr<Texture> createSharedPtr(const std::string name, uint32_t id, uint32_t width, uint32_t height, uint32_t bpp, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap);
        static std::unique_ptr<Texture> createUniquePtr(const std::string name, uint32_t id, uint32_t width, uint32_t height, uint32_t bpp, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap);
    };
}
}
}

#endif // ENGINE_OPEN_GL_RESOURCES_TEXTURE_HPP_
