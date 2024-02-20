#include "core/resources/material.hpp"

#include "core/resource_management/texture_manager.hpp"

namespace ResManagement = Engine::Core::ResourceManagement;

Resources::Material::~Material()
{
    if (m_diffuse.has_value())
    {
        ResManagement::TextureManager::Instance().Destroy(m_diffuse.value());
    }
}

std::shared_ptr<Resources::Material> Resources::Material::createSharedPtr(std::string name)
{
    Resources::Material* material = new Resources::Material(name);
    return std::shared_ptr<Resources::Material>(material);
}

std::unique_ptr<Resources::Material> Resources::Material::createUniquePtr(std::string name)
{
    Resources::Material* material = new Resources::Material(name);
    return std::unique_ptr<Resources::Material>(material);
}

void Resources::Material::Bind(const std::shared_ptr<OpenGL::Resources::Shader> shader)
{
    shader->Bind();

    int textureSlot = 0;

    ResManagement::TextureManager& texManager = ResManagement::TextureManager::Instance();

    bool test1 = m_diffuse.has_value();
    bool test2 = texManager.IsResourceLoaded(m_diffuse.value());
    bool test3 = shader->GetUniformInfo("diffuse").has_value();

    if (m_diffuse.has_value() && texManager.IsResourceLoaded(m_diffuse.value())
        && shader->GetUniformInfo("diffuse").has_value())
    {
        texManager[m_diffuse.value()]->Bind(textureSlot);
        shader->SetUniformInt("diffuse", textureSlot++);
    }
}

void Resources::Material::UnBind(const std::shared_ptr<OpenGL::Resources::Shader> shader)
{
    shader->Unbind();
}

void Resources::Material::SetDepthTest(bool depthTest)
{
    m_depthTest = depthTest;
}

void Resources::Material::SetDiffuse(std::string name)
{
    m_diffuse = name;
}

bool Resources::Material::HasDepthTest() const
{
    return m_depthTest;
}

uint8_t Resources::Material::GenerateStateMask() const
{
    return 0b0001'0000;
}
