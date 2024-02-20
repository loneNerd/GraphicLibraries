#pragma once
#ifndef ENGINE_TOOLS_UTILS_SERVICE_LOCATOR_HPP_
#define ENGINE_TOOLS_UTILS_SERVICE_LOCATOR_HPP_

#include <memory>
#include <unordered_map>
#include <type_traits>

namespace Engine::Tools::Utils
{
    class ServiceLocator
    {
    public:
        template<typename T, typename... Args>
        static std::shared_ptr<T> Provide(Args&&... args)
        {
            int hash = static_cast<int>(typeid(T).hash_code());

            if (m_services.find(hash) == m_services.end())
                m_services.emplace(hash, std::make_shared<T>(args...));

            return std::static_pointer_cast<T>(m_services.find(static_cast<int>(typeid(T).hash_code()))->second);
        }

        template<typename T>
        static inline const std::shared_ptr<T> Get()
        {
            auto instance = m_services.find(static_cast<int>(typeid(T).hash_code()));

            if (instance != m_services.end())
                return std::static_pointer_cast<T>(instance->second);

            return nullptr;
        }

    private:
        static inline std::unordered_map<size_t, std::shared_ptr<void>> m_services;
    };
}

#endif // ENGINE_TOOLS_UTILS_SERVICE_LOCATOR_HPP_
