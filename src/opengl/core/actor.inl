#pragma once
#ifndef ENGINE_CORE_ACTOR_INL_
#define ENGINE_CORE_ACTOR_INL_

#include "actor.hpp"

namespace Core = Engine::Core;

template<typename T, typename... Args>
inline std::shared_ptr<T> Core::Actor::AddComponent(Args&&... args)
{
    static_assert(std::is_base_of<Components::Component, T>::value, "T should derive from Component");

    std::shared_ptr<T> result = GetComponent<T>();

    if (!result)
    {
        std::shared_ptr<T> instance = std::make_shared<T>(args...);
        m_components.emplace(m_components.begin(), instance);
        return instance;
    }
    else
        return result;
}

template<typename T>
inline bool Core::Actor::RemoveComponent()
{
    static_assert(std::is_base_of<Components::Component, T>::value, "T should derive from Component");
    static_assert(!std::is_same<Components::ComponentTransform, T>::value, "You can't remove a Transform from an actor");

    std::shared_ptr<T> result = nullptr;

    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        result = std::dynamic_pointer_cast<T>(*it);
        if (result)
        {
            m_components.erase(it);
            return true;
        }
    }

    return false;
}

template<typename T>
inline std::shared_ptr<T> Core::Actor::GetComponent()
{
    static_assert(std::is_base_of<Components::Component, T>::value, "T should derive from Component");

    std::shared_ptr<T> result = nullptr;

    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        result = std::dynamic_pointer_cast<T>(*it);
        if (result)
            return result;
    }

    return nullptr;
}

#endif // ENGINE_CORE_ACTOR_INL_
