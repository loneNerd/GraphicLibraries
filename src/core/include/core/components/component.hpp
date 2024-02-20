#pragma once
#ifndef ENGINE_CORE_COMPONENTS_COMPONENT_HPP_
#define ENGINE_CORE_COMPONENTS_COMPONENT_HPP_

#include <memory>
#include <string>

namespace Engine::Core::Components
{
    class Component
    {
    public:
        Component() { }
        virtual ~Component() { }

        Component(const Component& other)             = delete;
        Component(Component&& other)                  = delete;
        Component& operator=(const Component& other)  = delete;
        Component& operator=(const Component&& other) = delete;

        virtual void OnUpdate(float dt) {}
        virtual std::string GetName() = 0;
    };
}

#endif // ENGINE_CORE_COMPONENTS_COMPONENT_HPP_
