#pragma once
#ifndef ENGINE_CORE_COMPONENTS_MATERIAL_RENDERER_HPP_
#define ENGINE_CORE_COMPONENTS_MATERIAL_RENDERER_HPP_

#include <array>

#include "component.hpp"
#include "core/resources/material.hpp"

#define MAX_MATERIAL_COUNT 255

namespace Engine::Core::Components
{
    class MaterialRenderer : public Component
    {
    public:
        using MaterialList = std::array<std::shared_ptr<Resources::Material>, MAX_MATERIAL_COUNT>;

        MaterialRenderer();

        MaterialRenderer(const MaterialRenderer& other)             = delete;
        MaterialRenderer(MaterialRenderer&& other)                  = delete;
        MaterialRenderer& operator=(const MaterialRenderer& other)  = delete;
        MaterialRenderer& operator=(const MaterialRenderer&& other) = delete;

        std::string GetName() override;
        void FillWithMaterial(std::shared_ptr<Resources::Material> material);
        void SetMaterialAtIndex(uint8_t index, std::shared_ptr<Resources::Material> material);
        std::shared_ptr<Resources::Material> GetMaterialAtIndex(uint8_t index);
        void RemoveMaterialAtIndex(uint8_t index);
        void RemoveMaterialByInstance(std::shared_ptr<Resources::Material> instance);
        void RemoveAllMaterials();
        void SetUserMatrixElement(uint32_t row, uint32_t column, float value);
        float GetUserMatrixElement(uint32_t row, uint32_t column) const;
        const Math::FMatrix4& GetUserMatrix() const;
        const MaterialList& GetMaterials() const;

    private:
        MaterialList m_materials;
        Math::FMatrix4 m_userMatrix;
    };
}

#endif // ENGINE_CORE_COMPONENTS_MATERIAL_RENDERER_HPP_
