#include "texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace FS = std::filesystem;

namespace Settings = Engine::OpenGL::Settings;
namespace ResManagement = Engine::Core::ResourceManagement;
namespace OGLResources = Engine::OpenGL::Resources;

std::shared_ptr<OGLResources::Texture> ResManagement::TextureManager::Create(FS::path filePath, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap, bool flip)
{
    std::string name = filePath.string();

    if (IsResourceLoaded(name))
        return GetResource(name);

    GLuint textureID;
    int textureWidth;
    int textureHeight;
    int bitsPerPixel;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(flip);
    unsigned char* dataBuffer = stbi_load(name.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

    if (dataBuffer)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);

        if (generateMipmap)
            glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(firstFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(secondFilter));

        stbi_image_free(dataBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        auto newTexture = m_resources.emplace(name, OGLResources::Texture::createSharedPtr(name, textureID, textureWidth, textureHeight, bitsPerPixel, firstFilter, secondFilter, generateMipmap));
        return newTexture.first->second;
    }
    else
    {
        stbi_image_free(dataBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
        return nullptr;
    }
}

std::shared_ptr<OGLResources::Texture> ResManagement::TextureManager::CreateColor(std::string name, uint32_t data, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);

    if (generateMipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(firstFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(secondFilter));

    glBindTexture(GL_TEXTURE_2D, 0);

    auto newTexture = m_resources.emplace(name, OGLResources::Texture::createSharedPtr(name, textureID, 1, 1, 32, firstFilter, secondFilter, generateMipmap));
    return newTexture.first->second;
}

std::shared_ptr<OGLResources::Texture> ResManagement::TextureManager::CreateFromMemory(std::string name, uint8_t* data, uint32_t width, uint32_t height, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (generateMipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(firstFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(secondFilter));

    glBindTexture(GL_TEXTURE_2D, 0);

    auto newTexture = m_resources.emplace(name, OGLResources::Texture::createSharedPtr(name, textureID, 1, 1, 32, firstFilter, secondFilter, generateMipmap));
    return newTexture.first->second;
}

void ResManagement::TextureManager::Reload(std::shared_ptr<OGLResources::Texture> texture, FS::path filePath, Settings::ETextureFilteringMode firstFilter, Settings::ETextureFilteringMode secondFilter, bool generateMipmap, bool flip)
{
    glDeleteTextures(1, &texture->Id);
    RemoveResource(texture->Name);
    std::shared_ptr<OGLResources::Texture> newTexture = Create(filePath, firstFilter, secondFilter, generateMipmap, flip);
    texture = newTexture;
}

void ResManagement::TextureManager::Destroy(const std::string& name)
{
    if (IsResourceLoaded(name) && m_resources[name].use_count() < 2)
    {
        RemoveResource(name);
    }
}
