#include "core/actor.hpp"

#include <algorithm>

namespace Core = Engine::Core;

Core::Actor::Actor(int64_t actorID, const std::string& name, const std::string& tag)
    : m_actorID(actorID),
      m_name(name),
      m_tag(tag)
{
    m_transform = AddComponent<Components::ComponentTransform>();
}

Core::Actor::~Actor()
{
}

const std::string& Core::Actor::GetName() const
{
    return m_name;
}

const std::string& Core::Actor::GetTag() const
{
    return m_tag;
}

void Core::Actor::SetName(const std::string& name)
{
    m_name = name;
}

void Core::Actor::SetTag(const std::string& tag)
{
    m_tag = tag;
}

void Core::Actor::SetID(int64_t id)
{
    m_actorID = id;
}

int64_t Core::Actor::GetID() const
{
    return m_actorID;
}

void Core::Actor::OnUpdate(float dt)
{

}

bool Core::Actor::RemoveComponent(std::shared_ptr<Components::Component> component)
{
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        if (*it == component)
        {
            m_components.erase(it);
            return true;
        }
    }

    return false;
}

std::list<std::shared_ptr<Core::Components::Component>>& Core::Actor::GetComponents()
{
    return m_components;
}
