#include "material_renderer.hpp"

namespace Components = Engine::Core::Components;
namespace Resources = Engine::Core::Resources;
namespace Math = Engine::Math;

Components::MaterialRenderer::MaterialRenderer()
{
    m_materials.fill(nullptr);
}

std::string Components::MaterialRenderer::GetName()
{
    return "Material Renderer";
}

void Components::MaterialRenderer::FillWithMaterial(std::shared_ptr<Resources::Material> material)
{
    for (uint8_t i = 0; i < m_materials.size(); ++i)
        m_materials[i] = material;
}

void Components::MaterialRenderer::SetMaterialAtIndex(uint8_t index, std::shared_ptr<Resources::Material> material)
{
    m_materials[index] = material;
}

std::shared_ptr<Resources::Material> Components::MaterialRenderer::GetMaterialAtIndex(uint8_t index)
{
    return m_materials.at(index);
}

void Components::MaterialRenderer::RemoveMaterialAtIndex(uint8_t index)
{
    if (index < m_materials.size())
    {
        m_materials[index] = nullptr;;
    }
}

void Components::MaterialRenderer::RemoveMaterialByInstance(std::shared_ptr<Resources::Material> instance)
{
    for (uint8_t i = 0; i < m_materials.size(); ++i)
        if (m_materials[i] == instance)
            m_materials[i] = nullptr;
}

void Components::MaterialRenderer::RemoveAllMaterials()
{
    for (uint8_t i = 0; i < m_materials.size(); ++i)
        m_materials[i] = nullptr;
}

const Math::FMatrix4& Components::MaterialRenderer::GetUserMatrix() const
{
    return m_userMatrix;
}

const Components::MaterialRenderer::MaterialList& Components::MaterialRenderer::GetMaterials() const
{
    return m_materials;
}

void Components::MaterialRenderer::SetUserMatrixElement(uint32_t row, uint32_t column, float value)
{
    if (row < 4 && column < 4)
        m_userMatrix.Data[4 * row + column] = value;
}

float Components::MaterialRenderer::GetUserMatrixElement(uint32_t row, uint32_t column) const
{
    if (row < 4 && column < 4)
        return m_userMatrix.Data[4 * row + column];
    else
        return 0.0f;
}
