#include "texture.hpp"

#include <GL/glew.h>

namespace FS = std::filesystem;
namespace Resources = Engine::OpenGL::Resources;
namespace Settings = Engine::OpenGL::Settings;

Resources::Texture::~Texture()
{
    glDeleteTextures(1, &Id);
}

std::shared_ptr<Resources::Texture> Resources::Texture::createSharedPtr(std::string name, uint32_t id, uint32_t width, uint32_t height, uint32_t bpp, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap)
{
    Resources::Texture* texture = new Texture(name, id, width, height, bpp, firstFilter, secondFilter, generateMipmap);
    return std::shared_ptr<Resources::Texture>(texture);
}

std::unique_ptr<Resources::Texture> Resources::Texture::createUniquePtr(std::string name, uint32_t id, uint32_t width, uint32_t height, uint32_t bpp, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap)
{
    Resources::Texture* texture = new Texture(name, id, width, height, bpp, firstFilter, secondFilter, generateMipmap);
    return std::unique_ptr<Resources::Texture>(texture);
}

void Resources::Texture::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, Id);
}

void Resources::Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Resources::Texture::Texture(std::string name, uint32_t id, uint32_t width, uint32_t height, uint32_t bpp, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap) :
    Name(name), Id(id), Width(width), Height(height), BitsPerPixel(bpp), FirstFilter(firstFilter), SecondFilter(secondFilter), IsMimapped(generateMipmap)
{

}
