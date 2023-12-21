#pragma once
#ifndef ENGINE_CORE_RESOURCES_MATERIAL_INL_
#define ENGINE_CORE_RESOURCES_MATERIAL_INL_

#include "material.hpp"

namespace Resources = Engine::Core::Resources;

template<typename T>
inline void Resources::Material::Set(const std::string& key, const T& value)
{
    if (HasShader())
    {
        if (m_uniformsData.find(key) != m_uniformsData.end())
            m_uniformsData[key] = std::any(value);
    }
}

template<typename T>
inline const T& Resources::Material::Get(const std::string& key)
{
    if (m_uniformsData.find(key) != m_uniformsData.end())
        return T();
    else
        return std::any_cast<T>(m_uniformsData.at(key));
}

#endif // ENGINE_CORE_RESOURCES_MATERIAL_INL_
