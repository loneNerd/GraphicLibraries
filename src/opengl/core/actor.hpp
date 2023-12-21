#pragma once
#ifndef ENGINE_CORE_ACTOR_HPP_
#define ENGINE_CORE_ACTOR_HPP_

#include "components/component.hpp"
#include "components/component_transform.hpp"

namespace Engine
{
namespace Core
{
    class Actor
    {
    public:
        Actor(int64_t actorID, const std::string& name, const std::string& tag);
        virtual ~Actor();

        Actor(const Actor& other)             = delete;
        Actor(Actor&& other)                  = delete;
        Actor& operator=(const Actor& other)  = delete;
        Actor& operator=(const Actor&& other) = delete;

        const std::string& GetName() const;
        const std::string& GetTag() const;
        void SetName(const std::string& name);
        void SetTag(const std::string& tag);
        void SetID(int64_t id);
        int64_t GetID() const;
        void OnUpdate(float dt);

        template<typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args);

        template<typename T>
        bool RemoveComponent();

        bool RemoveComponent(std::shared_ptr<Components::Component> component);

        template<typename T>
        std::shared_ptr<T> GetComponent();

        std::list<std::shared_ptr<Components::Component>>& GetComponents();

        std::shared_ptr<Components::ComponentTransform> GetTransform() const { return m_transform; }

    private:
        std::string m_name = "";
        std::string m_tag = "";

        int64_t m_actorID = 0;

        std::list<std::shared_ptr<Components::Component>> m_components;
        std::shared_ptr<Components::ComponentTransform> m_transform;
    };
}
}

#include "actor.inl"

#endif // ENGINE_CORE_ACTOR_HPP_
