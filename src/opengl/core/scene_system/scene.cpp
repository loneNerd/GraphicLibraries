#include "scene.hpp"

#include <algorithm>

namespace SceneSystem = Engine::Core::SceneSystem;
namespace Core = Engine::Core;

SceneSystem::Scene::~Scene()
{
    std::for_each(m_actors.begin(), m_actors.end(), [](std::shared_ptr<Actor> element)
        {
            element = nullptr;
        });

    m_actors.clear();
}

void SceneSystem::Scene::Update(float dt)
{
    auto actors = m_actors;
    std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Actor::OnUpdate), std::placeholders::_1, dt));
}

std::shared_ptr<Core::Actor> SceneSystem::Scene::CreateActor()
{
    return CreateActor("New Actor");
}

std::shared_ptr<Core::Actor> SceneSystem::Scene::CreateActor(const std::string& name, const std::string& tag)
{
    m_actors.push_back(std::make_shared<Actor>(m_availableID++, name, tag));
    std::shared_ptr<Actor> instance = m_actors.back();
    return instance;
}

bool SceneSystem::Scene::DestroyActor(std::shared_ptr<Actor> target)
{
    auto found = std::find_if(m_actors.begin(), m_actors.end(), [target](std::shared_ptr<Actor> element)
        {
            return element == target;
        });

    if (found != m_actors.end())
    {
        *found = nullptr;
        m_actors.erase(found);
        return true;
    }
    else
    {
        return false;
    }
}

void SceneSystem::Scene::CollectGarbages()
{
}

std::shared_ptr<Core::Actor> SceneSystem::Scene::FindActorByName(const std::string& name)
{
    auto result = std::find_if(m_actors.begin(), m_actors.end(), [name](std::shared_ptr<Actor> element)
        {
            return element->GetName() == name;
        });

    if (result != m_actors.end())
        return *result;
    else
        return nullptr;
}

std::shared_ptr<Core::Actor> SceneSystem::Scene::FindActorByTag(const std::string& tag)
{
    auto result = std::find_if(m_actors.begin(), m_actors.end(), [tag](std::shared_ptr<Actor> element)
        {
            return element->GetTag() == tag;
        });

    if (result != m_actors.end())
        return *result;
    else
        return nullptr;
}

std::shared_ptr<Core::Actor> SceneSystem::Scene::FindActorByID(int64_t id)
{
    auto result = std::find_if(m_actors.begin(), m_actors.end(), [id](std::shared_ptr<Actor> element)
        {
            return element->GetID() == id;
        });

    if (result != m_actors.end())
        return *result;
    else
        return nullptr;
}

std::vector<std::shared_ptr<Core::Actor>> SceneSystem::Scene::FindActorsByName(const std::string& name)
{
    std::vector<std::shared_ptr<Actor>> actors;

    for (auto actor : m_actors)
    {
        if (actor->GetName() == name)
            actors.push_back(actor);
    }

    return actors;
}

std::vector<std::shared_ptr<Core::Actor>> SceneSystem::Scene::FindActorsByTag(const std::string& tag)
{
    std::vector<std::shared_ptr<Actor>> actors;

    for (auto actor : m_actors)
    {
        if (actor->GetTag() == tag)
            actors.push_back(actor);
    }

    return actors;
}

std::vector<std::shared_ptr<Core::Actor>>& SceneSystem::Scene::GetActors()
{
    return m_actors;
}
